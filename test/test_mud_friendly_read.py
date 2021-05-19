# Test mud_friendly
# Derek Fujimoto
# May 2021

from mudpy.mud_friendly import *
from numpy.testing import *
import requests, os, time

run = 40033
year = 2019
area = 'BNMR'
    
def get_file(run, year, area='BNMR'):
    
    # fetch data
    filename = '0%d.msr' % run
    url = 'http://musr.ca/mud/data/%s/%d/%s' % (area, year, filename)
    webfile = requests.get(url)
    if not webfile.ok:
        raise RuntimeError('File %s not found. '%url+\
                    'Attempted download from musr.ca failed.')
    
    # write to file
    with open(filename, 'wb') as fid:
        fid.write(webfile.content)

    return filename
    
def get_header(run, year, area='BNMR'):
    
    # fetch data
    filename = '0%d.msr' % run
    url = 'http://musr.ca/mud/mud_hdrs.php?cmd=heads&fn=data/%s/%d/%s' % \
            (area, year, filename)
    webfile = requests.get(url)
    
    if not webfile.ok:
        raise RuntimeError('File %s not found. '%url+\
                    'Attempted download from musr.ca failed.')
    
    # get header lines
    lines = webfile.content.decode().splitlines()
    
    # clean html
    lines = [l for l in lines if l != '' and l[0] != '<' and l[1] != '<']
    
    return lines

def get_descr(lines):
    
    # make dict of values
    values = {}
    do_title = False
    do_histogram = 0
    do_variable = 0
    for line in lines:
        
        # clean
        line = line.strip()
        
        # check title (next line)
        if do_title:
            do_title = False
            values['title'] = line
            continue
            
        if 'Scaler' in line:
            break
            
        elif 'title' in line:
            do_title = True 
            
        # normal header split
        elif ':' in line:
            
            lst = line.split(':')
            key = lst[0]
            val = ':'.join(lst[1:])
            values[key.strip()] = val.strip()
        
    return values

def get_obj(lines, obj):
    """
        obj = Hist or Independent variable
    """
    
    # make dict of values
    values = {}
    do_histogram = 0
    for line in lines:
        
        # clean
        line = line.strip()
        
        if do_histogram:
            lst = line.split(',')
            
            for l in lst:
                l2 = l.split(':')
                values[hist_title][l2[0].strip()] = l2[1].strip()
            
            do_histogram -= 1
        
        # histogram
        if obj in line:
            do_histogram = 3
            lst = line.split(':')
            hist_title = (':'.join(lst[1:])).strip()
            values[hist_title] = {}
            
    return values

def get_scaler(lines):
    
    # make dict of values
    values = {}
    for line in lines:
        
        # clean
        line = line.strip()
        
        # get
        if 'Scaler' in line:
            line = ' '.join(line.split()[1:])
            title = line.split('total')[0].strip()
            line = line.replace(title, '').strip()
            lst = line.split(',')
            total = lst[0].split(':')[1].strip()
            recent = lst[1].split(':')[1].strip()
            values[title] = {'total': total, 'recent': recent}
            
    return values
    
def run_with_file(function):
    
    def wrapper(*args, **kwargs):
        
        # get the data
        filename = get_file(run, year, area)
        
        # run test
        try:
            return function(*args, **kwargs, filename=filename)
        
        # clean up
        finally:
            os.remove(filename)
                
    return wrapper

def run_with_handle(function):
    
    def wrapper(*args, **kwargs):
        
        # get the data
        filename = get_file(run, year, area)
        fh = open_read(filename)
        
        # run test
        try:
            return function(*args, **kwargs, fh=fh)
        
        # clean up
        finally:
            close_read(fh)
            os.remove(filename)
            
    return wrapper

# test reading description ----------------------------------------------------
@run_with_file
def test_open_close_read(filename):
    fh = open_read(filename)
    close_read(fh)

@run_with_handle
def test_get_descr(fh):
    values = get_descr(get_header(run, year))
    val = get_description(fh)
    head = SEC_GEN_RUN_DESC_ID
    assert val == head, '%s != %s' % (val, head)

@run_with_handle
def test_get_exp_number(fh):
    values = get_descr(get_header(run, year))
    assert get_exp_number(fh) == int(values['exper num'])

@run_with_handle
def test_get_run_number(fh):
    values = get_descr(get_header(run, year))
    val = get_run_number(fh)
    head = int(values['Run number'])
    assert val == head, '%s != %s' % (val, head)

@run_with_handle
def test_get_elapsed_seconds(fh):
    values = get_descr(get_header(run, year))
    head = int(values['elapsed'].split('(')[1].split(' ')[0])
    val = get_elapsed_seconds(fh)
    assert val == head, '%s != %s' % (val, head)
    
@run_with_handle
def test_get_start_time(fh):
    values = get_descr(get_header(run, year))
    val = time.ctime(get_start_time(fh))
    head = values['began']
    assert val == head, '%s != %s' % (val, head)

@run_with_handle
def test_get_end_time(fh):
    values = get_descr(get_header(run, year))
    val = time.ctime(get_end_time(fh))
    head = values['ended']
    assert val == head, '%s != %s' % (val, head)
    
@run_with_handle
def test_get_title(fh):
    values = get_descr(get_header(run, year))
    val = get_title(fh)
    head = values['title']
    assert val == head, '%s != %s' % (val, head)
    
@run_with_handle
def test_get_lab(fh):
    values = get_descr(get_header(run, year))
    val = get_lab(fh)
    head = values['lab']
    assert val == head, '%s != %s' % (val, head)
    
@run_with_handle
def test_get_area(fh):
    values = get_descr(get_header(run, year))
    val = get_area(fh)
    head = values['area']
    assert val == head, '%s != %s' % (val, head)
    
@run_with_handle
def test_get_method(fh):
    values = get_descr(get_header(run, year))
    val = get_method(fh)
    head = values['method']
    assert val == head, '%s != %s' % (val, head)
    
@run_with_handle
def test_get_apparatus(fh):
    values = get_descr(get_header(run, year))
    val = get_apparatus(fh)
    head = values['apparatus']
    assert val == head, '%s != %s' % (val, head)
    
@run_with_handle
def test_get_insert(fh):
    values = get_descr(get_header(run, year))
    val = get_insert(fh)
    head = values['insert']
    assert val == head, '%s != %s' % (val, head)
    
@run_with_handle
def test_get_sample(fh):
    values = get_descr(get_header(run, year))
    val = get_sample(fh)
    head = values['sample']
    assert val == head, '%s != %s' % (val, head)
    
@run_with_handle
def test_get_orientation(fh):
    values = get_descr(get_header(run, year))
    val = get_orientation(fh)
    head = values['orient']
    assert val == head, '%s != %s' % (val, head)
    
@run_with_handle
def test_get_das(fh):
    values = get_descr(get_header(run, year))
    val = get_das(fh)
    head = values['das']
    assert val == head, '%s != %s' % (val, head)
    
@run_with_handle
def test_get_experimenter(fh):
    values = get_descr(get_header(run, year))
    val = get_experimenter(fh)
    head = values['operator']
    assert val == head, '%s != %s' % (val, head)
    
@run_with_handle
def test_get_temperature(fh):
    values = get_descr(get_header(run, year))
    val = get_temperature(fh)
    head = values['temperature']
    assert val == head, '%s != %s' % (val, head)
    
@run_with_handle
def test_get_field(fh):
    values = get_descr(get_header(run, year))
    val = get_field(fh)
    head = values['field']
    assert val == head, '%s != %s' % (val, head)
    
# test reading histogram headers ----------------------------------------------

@run_with_handle
def test_get_hists(fh):
    values = get_obj(get_header(run, year), 'Hist')
    val = get_hists(fh)[1]
    head = len(list(values.keys()))
    assert val == head, '%s != %s' % (val, head)
    
@run_with_handle
def test_get_hist_type(fh):
    values = get_obj(get_header(run, year), 'Hist')
    val = get_hist_type(fh, 1)
    head = SEC_TRI_TD_HIST_ID
    assert val == head, '%s != %s' % (val, head)
    
@run_with_handle
def test_get_hist_title(fh):
    values = get_obj(get_header(run, year), 'Hist')
    val = get_hist_title(fh, 1)
    assert val in values.keys()

@run_with_handle
def test_get_hist_n_bins(fh):
    values = get_obj(get_header(run, year), 'Hist')
    title = get_hist_title(fh, 1)
    val = get_hist_n_bins(fh, 1)
    head = int(values[title]['nBins'])
    assert val == head, '%s != %s' % (val, head)

@run_with_handle
def test_get_hist_fs_per_bin(fh):
    values = get_obj(get_header(run, year), 'Hist')
    title = get_hist_title(fh, 1)
    val = get_hist_fs_per_bin(fh, 1) * 1e-6
    head = float(values[title]['ns_per_Bin'])
    assert val == head, '%s != %s' % (val, head)

@run_with_handle
def test_get_hist_t0_ps(fh):
    values = get_obj(get_header(run, year), 'Hist')
    title = get_hist_title(fh, 1)
    val = get_hist_t0_ps(fh, 1)
    head = int(values[title]['t0_ps'])
    assert val == head, '%s != %s' % (val, head)

@run_with_handle
def test_get_hist_t0_bin(fh):
    values = get_obj(get_header(run, year), 'Hist')
    title = get_hist_title(fh, 1)
    val = get_hist_t0_bin(fh, 1)
    head = int(values[title]['t0_bin'])
    assert val == head, '%s != %s' % (val, head)

@run_with_handle
def test_get_hist_good_bin1(fh):
    values = get_obj(get_header(run, year), 'Hist')
    title = get_hist_title(fh, 1)
    val = get_hist_good_bin1(fh, 1)
    head = int(values[title]['goodBin1'])
    assert val == head, '%s != %s' % (val, head)

@run_with_handle
def test_get_hist_good_bin2(fh):
    values = get_obj(get_header(run, year), 'Hist')
    title = get_hist_title(fh, 1)
    val = get_hist_good_bin2(fh, 1)
    head = int(values[title]['goodBin2'])
    assert val == head, '%s != %s' % (val, head)

@run_with_handle
def test_get_hist_background1(fh):
    values = get_obj(get_header(run, year), 'Hist')
    title = get_hist_title(fh, 1)
    val = get_hist_background1(fh, 1)
    head = int(values[title]['bkgd1'])
    assert val == head, '%s != %s' % (val, head)

@run_with_handle
def test_get_hist_background2(fh):
    values = get_obj(get_header(run, year), 'Hist')
    title = get_hist_title(fh, 1)
    val = get_hist_background2(fh, 1)
    head = int(values[title]['bkgd2'])
    assert val == head, '%s != %s' % (val, head)

@run_with_handle
def test_get_hist_n_events(fh):
    values = get_obj(get_header(run, year), 'Hist')
    title = get_hist_title(fh, 1)
    val = get_hist_n_events(fh, 1)
    head = int(values[title]['nEvents'])
    assert val == head, '%s != %s' % (val, head)
        
# test reading scaler headers -------------------------------------------------

@run_with_handle
def test_get_scalers(fh):
    values = get_scaler(get_header(run, year))
    val = get_scalers(fh)[1]
    head = len(list(values.keys()))
    assert val == head, '%s != %s' % (val, head)

@run_with_handle
def test_get_scaler_label(fh):
    values = get_scaler(get_header(run, year))
    val = get_scaler_label(fh, 1)
    assert val in values.keys()

@run_with_handle
def test_get_scaler_counts(fh):
    values = get_scaler(get_header(run, year))
    title = get_scaler_label(fh, 1)
    val = get_scaler_counts(fh, 1)
    head = values[title]
    
    assert_almost_equal(val[0], int(head['total']), err_msg='%s != %s' % (val[0], head['total']))
    assert_almost_equal(val[1], int(head['recent']), err_msg='%s != %s' % (val[1], head['recent']))

# test reading variable headers -----------------------------------------------

@run_with_handle
def test_get_ivars(fh):
    values = get_obj(get_header(run, year), 'Independent variable')
    val = get_ivars(fh)[1]
    head = len(list(values.keys()))
    assert val == head, '%s != %s' % (val, head)
    
@run_with_handle
def test_get_ivar_name(fh):
    values = get_obj(get_header(run, year), 'Independent variable')
    val = get_ivar_name(fh, 1)
    assert val in values.keys()

@run_with_handle
def test_get_ivar_low(fh):
    values = get_obj(get_header(run, year), 'Independent variable')
    title = get_ivar_name(fh, 1)
    val = get_ivar_low(fh, 1)
    head = float(values[title]['low'])
    assert_almost_equal(val, head, err_msg='%s != %s' % (val, head))
    
@run_with_handle
def test_get_ivar_high(fh):
    values = get_obj(get_header(run, year), 'Independent variable')
    title = get_ivar_name(fh, 1)
    val = get_ivar_high(fh, 1)
    head = float(values[title]['high'])
    assert_almost_equal(val, head, err_msg='%s != %s' % (val, head))
    
@run_with_handle
def test_get_ivar_mean(fh):
    values = get_obj(get_header(run, year), 'Independent variable')
    title = get_ivar_name(fh, 1)
    val = get_ivar_mean(fh, 1)
    head = float(values[title]['mean'])
    assert_almost_equal(val, head, err_msg='%s != %s' % (val, head))
    
@run_with_handle
def test_get_ivar_std(fh):
    values = get_obj(get_header(run, year), 'Independent variable')
    title = get_ivar_name(fh, 1)
    val = get_ivar_std(fh, 1)
    head = float(values[title]['stddev'])
    assert_almost_equal(val, head, err_msg='%s != %s' % (val, head))
    
@run_with_handle
def test_get_ivar_skewness(fh):
    values = get_obj(get_header(run, year), 'Independent variable')
    title = get_ivar_name(fh, 1)
    val = get_ivar_skewness(fh, 1)
    head = float(values[title]['skewness'])
    assert_almost_equal(val, head, err_msg='%s != %s' % (val, head))
    
@run_with_handle
def test_get_ivar_description(fh):
    values = get_obj(get_header(run, year), 'Independent variable')
    title = get_ivar_name(fh, 1)
    val = get_ivar_description(fh, 1)
    head = values[title]['description']
    assert val == head, '%s != %s' % (val, head)

@run_with_handle
def test_get_ivar_units(fh):
    values = get_obj(get_header(run, year), 'Independent variable')
    title = get_ivar_name(fh, 1)
    val = get_ivar_units(fh, 1)
    head = values[title]['units']
    assert val == head, '%s != %s' % (val, head)


