# mudpy

<a href="https://pypi.org/project/mud-py/" alt="PyPI Version"><img src="https://img.shields.io/pypi/v/mud-py?label=PyPI%20Version"/></a>
<img src="https://img.shields.io/pypi/format/mud-py?label=PyPI%20Format"/>
<img src="https://img.shields.io/github/languages/code-size/dfujim/mudpy"/>
<img src="https://img.shields.io/tokei/lines/github/dfujim/mudpy"/>
<img src="https://img.shields.io/pypi/l/mud-py"/>

<a href="https://github.com/dfujim/mudpy/commits/master" alt="Commits"><img src="https://img.shields.io/github/commits-since/dfujim/mudpy/latest/master"/></a>
<a href="https://github.com/dfujim/mudpy/commits/master" alt="Commits"><img src="https://img.shields.io/github/last-commit/dfujim/mudpy"/></a>

MUon (MUD) Data file IO package.

[mudpy] is a lightwieght [Python] package aimed to aid in the analysis of muon spin rotation ([μSR]) and β-detected
nuclear magnetic/quadrupole resonance (β-NMR and β-NQR) data taken at [TRIUMF]. These techniques are similar to 
"conventional" nuclear magnetic resonance ([NMR]), but use radioactive nuclei or a [muon] as the [NMR] probe in place 
of a stable isotope.

The intended user of [mudpy] is anyone analyzing data in [TRIUMF]'s [MUD] file format.
A key goal of the project is to alleviate much of the technical tedium that is often encountered during any analysis.

[mudpy] has been written to fullfill the following needs: 

* Provide a pythonic means of interfacing with [MUD] files in [Python].
* Provide low-level wrappers of the [`mud_friendly`] API. 

## Citing

If you use [mudpy] in your work, please cite:

- D. Fujimoto.
  <i>Digging Into MUD With Python: mudpy, bdata, and bfit</i>.
  <a href="https://arxiv.org/abs/2004.10395">
  arXiv:2004.10395 [physics.data-an]</a>.


## Community Guidelines

* Please submit contributions to [mudpy] via a pull request
* To report issues or get support, please file a new issue

## Installation and Use

### Dependencies

The following packages/applications are needed prior to installation:
- [Python] 3.6 or higher: a dynamically typed programming language. [[install](https://wiki.python.org/moin/BeginnersGuide/Download)]

### Install Instructions

|  | Command |
|:-- | :--|
From the [PyPI] as user (recommended) | `pip install --user mud-py` |
From the [PyPI] as root | `pip install mud-py` |
From source | `python3 setup.py install` |

Note the hyphen in `mud-py`. The `pip` command should point to a (version 3) 
[Python] executable (e.g., `python3`, `python3.8`, etc.).
If the above does not work, try using `pip3` or `python3 -m pip` instead.

## [Contents](https://github.com/dfujim/mudpy/wiki)

* [`mud_friendly`] [C wrapper]: python access to [MUD] C functions
* [`mdata`](https://github.com/dfujim/mudpy/wiki/mdata) [object]: access general [MUD] files pythonically
* [`containers.mcontainer`](https://github.com/dfujim/mudpy/wiki/containers.mcontainer) [object]: special container base class
* [`containers.mdict`](https://github.com/dfujim/mudpy/wiki/containers.mdict) [object]: enhanced dictionary class for sub-level lookup and attribute access
* [`containers.mcomment`](https://github.com/dfujim/mudpy/wiki/containers.mcomment) [object]: special container for comments
* [`containers.mhist`](https://github.com/dfujim/mudpy/wiki/containers.mhist) [object]: special container for histograms
* [`containers.mscaler`](https://github.com/dfujim/mudpy/wiki/containers.mscaler) [object]: special container for scalers
* [`containers.mlist`](https://github.com/dfujim/mudpy/wiki/containers.mlist) [object]: enhanced list class for sub-level lookup
* [`containers.mvar`](https://github.com/dfujim/mudpy/wiki/containers.mvar) [object]: special container for variables

[Python]: https://www.python.org/
[SciPy]: https://www.scipy.org/
[Cython]: https://cython.org/
[NumPy]: https://numpy.org/
[pandas]: https://pandas.pydata.org/
[Matplotlib]: https://matplotlib.org/
[requests]: https://requests.readthedocs.io/en/master/
[Jupyter]: https://jupyter.org/

[YAML]: https://yaml.org/
[C]: https://en.wikipedia.org/wiki/C_(programming_language)
[HTTP]: https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol

[TRIUMF]: https://www.triumf.ca/
[CMMS]: https://cmms.triumf.ca
[MUD]: https://cmms.triumf.ca/mud/
[archive]: https://cmms.triumf.ca/mud/runSel.html

[UBC]: https://www.ubc.ca/
[μSR]: https://en.wikipedia.org/wiki/Muon_spin_spectroscopy
[NMR]: https://en.wikipedia.org/wiki/Nuclear_magnetic_resonance
[muon]: https://en.wikipedia.org/wiki/Muon

[PyPI]: https://pypi.org/project/bdata/
[mudpy]: https://github.com/dfujim/mudpy
[bdata]: https://github.com/dfujim/bdata
[bfit]: https://github.com/dfujim/bfit

[`mud_friendly`]: https://github.com/dfujim/mudpy/wiki/mud_friendly
