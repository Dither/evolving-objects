![Evolving Objects logo](http://eodev.sourceforge.net/eo_logo.png)

C++ evolutionary computation components framework
====

**Shortcuts**

*   [Download](http://sourceforge.net/project/showfiles.php?group_id=9775)
*   [Tutorial](http://eodev.sourceforge.net/eo/tutorial/html/eoTutorial.html)
*   [FAQ](https://sourceforge.net/apps/trac/eodev/wiki/faq)
*   [EO documentation](http://eodev.sourceforge.net/eo/doc/html/index.html)
*   [EDO documentation](http://eodev.sourceforge.net/edo/doc/html/index.html)
*   [Project page](http://sourceforge.net/project/?group_id=9775)
*   [Contact us](https://lists.sourceforge.net/lists/listinfo/eodev-main)
*   [Submit a bug](https://sourceforge.net/apps/trac/eodev/wiki/WikiStart)
*   see also [ParadisEO](http://paradiseo.gforge.inria.fr/)


[![Get Evolving Objects at SourceForge.net. Fast, secure and Free Open Source software downloads](http://sflogo.sourceforge.net/sflogo.php?group_id=9775&type=8)](http://sourceforge.net/projects/eodev)

# Evolving Objects (EO): an Evolutionary Computation Framework

EO is _a template-based, ANSI-C++ evolutionary computation library_ which helps you to write your own stochastic optimization algorithms insanely fast.

Evolutionary algorithms forms a family of algorithms inspired by the theory of evolution, that solve various problems. They evolve a set of solutions to a given problem, in order to produce the best results. These are stochastic algorithms, because they iteratively use random processes. The vast majority of these methods are used to solve optimization problems, and may be also called "metaheuristics". They are also ranked among _computational intelligence_ methods, a domain close to artificial intelligence.

With the help of EO, you can easily design evolutionary algorithms that will find solutions to virtually all kind of hard optimization problems, from continuous to combinatorial ones.

[Download](http://sourceforge.net/projects/eodev/files/)

If you need _immediate support or have any question_, the best way to get answers is to [subscribe to the mailing list](https://lists.sourceforge.net/lists/listinfo/eodev-main) and send your email to [eodev-main@lists.sourceforge.net](mailto:eodev-main@lists.sourceforge.net)

Alternatively, you can join us on the official chatroom. You can try our [webchat interface](http://irc.lc/freenode/paradiseo), or if you already use IRC, you can directly connect to the [irc.freenode.org/#paradiseo](irc://irc.freenode.org/#paradiseo) multi-user chatroom with your favorite client.

# Features [](#Plan)
## Component-based framework

Designing an algorithm with EO consists in choosing what components you want to use for your specific needs, just as building a structure with Lego blocks.

If you have a classical problem for which available code exists (for example if you have a black-box problem with real-valued variables), you will just choose components to form an algorithm and connect it to your _fitness function_ (which computes the quality of a given solution).

If your problem is a bit more exotic, you will have to code a class that represents how your _individuals_ (a solution to your problem) are represented, and perhaps some _variations operators_, but most of the other operators (selection, replacement, stopping criteria, command-line interface, etc.) are already available in EO.

![](Evolutionary_algorithm.png) "A typical Evolutionary Algorithm, as seen from the EO point of vue − Johann Dréo − CC-BY-SA")

## Main Features

Examples of problems that you can solve with the help of EO:  
![](http://eodev.sourceforge.net/Gallagher_Gaussian_BBOB_f21.png)

"Continuous problems (here Gallagher's Gaussian 101-me Peaks function, Black Box Optimization Benchmark, f21 − COCO team)"

![](http://eodev.sourceforge.net/200px-Graph_betweenness.svg.png)

"Graph problems (here a Hue scale representing node betweenness on a graph − Claudio Rocchini − CC-BY)"

![](http://eodev.sourceforge.net/BatchGantt1.png)

"Planning problems (here a Gantt Chart for a Batch Process − Casilett − DP)"

*   Flexible design that permits to easily create virtually any algorithm
*   Solution representation for continuous and combinatorial problems:
    *   binary-strings,
    *   permutations,
    *   vectors,
    *   easily write your own,
    *   …
*   Several algorithm paradigms:
    *   evolution strategies,
    *   genetic algorithms,
    *   estimation of distribution,
    *   particle swarm optimization
    *   …
*   Many selection and replacement operators:
    *   rank-based,
    *   deterministic or stochastic tournaments,
    *   roulette,
    *   elitism,
    *   …
*   Ready-to-use variations operators:
    *   uniform initializer,
    *   gaussian mutation,
    *   subtree crossover,
    *   …
*   Easy combination of several operators:
    *   proportional combination,
    *   sequential call,
    *   …
*   Parallelization tools:
    *   Shared memory loops unrolling (with [OpenMP](http://openmp.org))
    *   Message passing parallelization (with [openMPI](http://www.open-mpi.org/)):
        *   map/reduce-like design, with operators choice, as in EO
        *   useful existing operators (parallel dynamic multi-start, static evaluations, …)
*   Portable and human-readable parameter files
*   Suspend and load population from files
*   Versatile checkpointing and logging:
    *   graphical display,
    *   file dump,
    *   various statistics,
    *   signal catching,
    *   …
*   Mersenne Twister random number generator (and various distributions)
*   No useless computation (sparing fitness call, functor-based calls)
*   Fast running speed, thanks to C++
*   _And more!_

## Portability

EO should work on Windows and any Un*x-like operating system with a standard-conforming C++ development system.

Recent versions of EO have been tested on the following platforms:

*   Linux x86 with GCC 3.x and 4.x
*   Linux x86_64 with GCC 3.x and GCC 4.x
*   MacOS X/Darwin PowerPC with GCC 3.x
*   MacOS X/Darwin x86 with GCC 4.x
*   Microsoft Windows using Cygwin's GCC 3.x (cygming special).
*   Microsoft Windows using Visual Studio 2003/2005; projects files are provided.
*   Solaris SPARC with GCC 3.x
*   Solaris x86 with GCC 3.x

Recent versions of EO uses the [CMake](http://www.cmake.org) portable build system, that permits to easily generate a build chain for your environment.

If you have tested EO on a system not listed here, please [let us know](mailto:eodev-main@lists.sourceforge.net?subject=test-report).

If you are working on a system with an older C++ compiler there is a good chance that eo-0.9.3z.1 works. It is tested on Linux with gcc-2.9x and several systems (IRIX, Solaris) with egcs.

## Presentations

A functional and "philosophical" overview of EO was presented at [EA'01 conference](http://www.cmap.polytechnique.fr/%7Eea01/). You can download [the paper](http://eodev.sourceforge.net/eo/doc/EO_EA2001.pdf) or [the slides](http://eodev.sourceforge.net/eo/doc/LeCreusot.pdf).

You can also read this [PowerPoint presentation](http://geneura.ugr.es/~jmerelo/GAPPT/index.html), that shows the EO philosophy. It includes a Visual Basic macro for evolving objects in Visual Basic for Applications.


If you want to understand the message-passing parallelization module, check the [introduction to eo::MPI](http://eodev.sourceforge.net/eo/tutorial/Parallelization/eompi.html) by Benjamin Bouvier.

Here is a list of some known publications that used EO:

*   J.J. Gilijamse, J. Küpper, S. Hoekstra, S.Y.T. van de Meerakker, G. Meijer, [Optimizing the Stark-decelerator beamline for the trapping of cold molecules using evolutionary strategies](http://dx.doi.org/10.1103/PhysRevA.73.063410), _Physical Review_, A **73**, 063410 (2006).  
    Also available at [_arXiv_ physics/0603108 (2006)](http://arxiv.org/abs/physics/0603108).
*   Johann Dreo, [Using Performance Fronts for Parameter Setting of Stochastic Metaheuristics](http://www.nojhan.net/pro/spip.php?article31), _Genetic and Evolutionary Computation Conference_, (2009).
*   Jacques Bibaï, Pierre Savéant, Marc Schoenauer, Vincent Vidal, [An Evolutionary Metaheuristic Based on State Decomposition for Domain-Independent Satisficing Planning](http://www.aaai.org/ocs/index.php/ICAPS/ICAPS10/paper/view/1414), _Twentieth International Conference on Automated Planning and Scheduling_ (2010).
*   Jacques Bibaï, Pierre Savéant, Marc Schoenauer, Vincent Vidal, <a href="">An Evolutionary Metaheuristic for Domain-Independent Satisficing Planning</a>, _Genetic and Evolutionary Computation Conference_ (2010). _Silver Medal at the Human-Competitive Competition_

# Code [](#Plan)

## Download

The current stable release is [EO 1.0](http://sourceforge.net/project/showfiles.php?group_id=9775). It supports any standard-compliant C++ compiler.

You can obtain the latest version directly via [GIT](https://sourceforge.net/scm/?type=git&group_id=9775) or [browse the repository online](http://eodev.git.sourceforge.net/git/gitweb.cgi?p=eodev/eodev;a=tree;f=eo;h=d93666f049104f360ffdff3af0bf54902e2feae1;hb=HEAD).

All releases can be obtained from the SourceForge [download area](http://sourceforge.net/project/showfiles.php?group_id=9775).

## License

EO is distributed under the [GNU Lesser General Public License](http://www.gnu.org/copyleft/lesser.html).

Note that this license places copyleft restrictions on a program created with EO, but does not apply these restrictions to other software that would links with the program.

## Documentation

The tutorial demonstrates that writing an evolutionary algorithm evolving your own structures is now _easy_, using ready-to-use template files. Although the tutorial has not been upgraded for some time now and refers to version 0.9.2 of EO, it nevertheless remains the best way to dive into EO. You can start by trying it on-line at [LRI](http://www.lri.fr/%7Emarc/EO/eo/tutorial/html/eoTutorial.html) or [SourceForge](http://eodev.sourceforge.net/eo/tutorial/html/eoTutorial.html), before [downloading it](http://www.lri.fr/%7Emarc/EO/). The tutorial is also included in the [released sources](http://sourceforge.net/project/showfiles.php?group_id=9775).

The latest [tutorial release](http://eodev.sourceforge.net/eo/tutorial/html/eoTutorial.html).

The complete code is also well documented and you can look at the generated [interface documentation](http://eodev.sourceforge.net/eo/doc/html/index.html).

The easiest way to create a complete new EO-project, even for new genomes, is to use the script provided in tutorial/Templates/; see the README in that directory and lesson 5 of the tutorial for detail.

# Development [](#Plan)

## Mailing-lists

EO is an open development effort; that is why we have created mailing lists to discuss future developments, solve technical problems, announce releases, publish patches, and discuss evolutionary computation in general. Browse the archives or join the [EO mailing lists](http://sourceforge.net/mail/?group_id=9775).

## Get involved

The following resources are available, thanks to sourceforge

*   [EO SourceForge Project Page](http://sourceforge.net/project/?group_id=9775)
*   [EO automatic documentation page at SF](http://eodev.sourceforge.net/eo/doc/html/index.html)  
*   [EO tutorial page at SF](http://eodev.sourceforge.net/eo/tutorial/html/eoTutorial.html)  
*   [Releases](https://sourceforge.net/project/showfiles.php?group_id=9775)
*   [Mailing Lists](http://sourceforge.net/mail/?group_id=9775)
*   [Message Forums](http://sourceforge.net/forum/?group_id=9775)
*   [Bug Submission and Tracking](https://sourceforge.net/apps/trac/eodev/wiki/WikiStart)
*   [Technical Support](https://sourceforge.net/projects/eodev/support)
*   [Code repository](https://sourceforge.net/scm/?type=git&group_id=9775)

## Related software

The softwares listed here are using EO, but they are not maintained by the EO team. They may not be free softwares or may even be outdated.

*   [ParadisEO](http://paradiseo.gforge.inria.fr) provides EO extensions for the flexible design of **single solution-based metaheuristics**, metaheuristics for **multi objective optimization** as well as **hybrid, parallel and distributed metaheuristics**.
*   [DegaX](http://geneura.ugr.es/~jmerelo/DegaX/) is an ActiveX control which embeds EO 0.8.4.
*   [EASEA](https://lsiit.u-strasbg.fr/easea/index.php/EASEA_platform) was a GUI that permits to build evolutionary algorithm with EO or the [GAlib](http://lancet.mit.edu/ga/). It is now a platform that allows program evolutionary algorithms on massively parallel many-core architectures.
*   [GUIDE](https://gforge.inria.fr/projects/guide) is a GUI that allows the generation of evolutionary algorithms. It can use EO or [ECJ](http://cs.gmu.edu/~eclab/projects/ecj/).

## Authors

EO was started by the [Geneura Team](http://geneura.ugr.es/) at the University of Granada, headed by [Juan Julián Merelo](http://geneura.ugr.es/%7Ejmerelo/). The [original Web site](http://geneura.ugr.es/~jmerelo/EO.orig.html) is also the only place where you will find old releases of EO (up to 0.8.7), but beware that it is not compatible at all with the current version.

The developement team has then been reinforced by [Maarten Keijzer](http://www.cs.vu.nl/~mkeijzer), the C++ wizzard, and [Marc Schoenauer](http://www.lri.fr/%7Emarc). Later came [Jeroen Eggermont](http://www.liacs.nl/%7Ejeggermo/), who, among other things, did a lot of work on GP, INRIA Dolphin Team, [Olivier König](mailto:okoenig@users.sourceforge.net), who did a lot of useful additions and cleaning of the code and [Jochen Küpper](http://www.jochen-kuepper.de), working on infrastructure maintenance.

The project is now maintained by [Johann Dréo](http://johann.dreo.fr), working on it with the help of [Caner Candan](http://caner.candan.fr). [Benjamin Bouvier](https://github.com/BenjBouv) have designed a whole new MPI parallelization module.

## Links

*   [COCO (COmparing Continuous Optimisers)](http://coco.gforge.inria.fr/), a platform for systematic and sound comparisons of real-parameter global optimisers. COCO provides benchmark function testbeds and tools for processing and visualizing data generated by one or several optimizers.
*   [The Hitch-Hiker's Guide to Evolutionary Computation](http://www.aip.de/~ast/EvolCompFAQ), FAQ for [comp.ai.genetic](news:comp.ai.genetic).
*   Wikipedia entries on [Evolutionary algorithms](http://en.wikipedia.org/wiki/Evolutionary_algorithm).
*   Charles Darwin: [The Origin of Species](http://en.wikipedia.org/wiki/The_Origin_of_Species).
