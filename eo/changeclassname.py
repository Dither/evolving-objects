#!/usr/bin/env python

import os,sys

DIRS = [ 'src', 'src/do', 'src/es', 'src/ga', 'src/gp', 'src/other', 'src/utils' ]
DIRS_IMPL = [ 'app/gprop', 'app/gpsymreg', 'app/mastermind', 'test' ]

FILES = []
CLASSES = ['ValueParam', 'UF', 'BF', 'BinOp', 'MonOp', 'QuadOp', 'Rng', 'RndGenerator', 'STLF', 'VlDelMutation', 'GeneCHooser', 'UniformGeneChooser', 'GeneDelChooser', 'VlAddMutation', 'VlAtomExchangeQuadOp', 'AtomExchange', 'VlUniformQuadOp', 'InnerExchangeQuadOp', 'UniformAtomExchange', 'Dummy', 'Bounds', 'Many', 'Deterministic', 'Plus', 'Truncate', 'GeneralReplacement', 'STLUF', 'STLBF', 'F<', 'Invalidate', 'Eval', 'Roulette', 'DynSGATransform', 'SequentialOp', 'QuadCloneOp', 'ProportionalOp', 'Pters', 'EliteSequentialSelect', 'DetTournamentWorthSelect', 'eoTournament', 'MinimizingFitness', 'MaximizingFitness', 'StochTournamentWorthSelect', 'Assembled', 'WeakElitistReplacement', 'GenerationalReplacement', 'SSGAStochTournamentReplacement', 'DetTournament', 'LinearRandomSplit', 'RandomSplit', 'LinearTruncateSplit', 'SSGAWorseReplacement', 'Limit', 'BestSelect', 'NoSelect', 'SSGA', 'Linear', 'Stoch', 'Random', 'ExpDecayingBoundModifier', 'Perf', 'Prop', 'MonCloneOp', 'SeqPopulator', 'TimeVaryingLoopEval', 'Particle', 'FirstIsBestInit', 'Class', 'EPR', 'NoElitism', 'Comma', 'StochTournament', 'Proportional', 'ExpDecayingBoundModifier', 'NoPerf', 'Quad', 'Elitism', 'MonGenOp', 'BinGenOp', 'QuadGenOp', 'DerivedTransform', 'SelBinGenOp', 'VecOp', 'eoEngine', 'FlOr', 'Max', 'Islands', 'DistEval', 'Min', 'Procedure', 'Unary', 'Binary', 'Bin', 'Counted', 'Timed', 'Incrementor', 'Dyn', 'Neg', 'Normal', 'Boolean', 'Average', 'Best', 'Stdev', 'Second', 'Nth', 'Sorted', 'General', 'Uniform', 'StdOut', 'Container', 'G3', 'GnuPlot', 'IsFeasible', 'Hamming', 'Fitness', 'FDC', 'Scalar', 'SBX', 'Hypercube', 'Arithmetic', 'Segment', 'Det', 'Double', 'ES', 'UBit', 'NPts', '1Pt', 'One', 'PBIL', 'StHoist', 'StDepth', 'Gp', 'StPoint', 'StBranch', 'StSub', 'Hoist', 'Collapse', 'Expansion', 'Point', 'Sub', 'External']

for d in DIRS:
    for f in os.listdir(d):
        if '.h' in f or '.cpp' in f:
            FILES.append(d + '/' + f)
            CLASSES.append(f[:-2])

for d in DIRS_IMPL:
    for f in os.listdir(d):
        if '.h' in f or '.cpp' in f:
            FILES.append(d + '/' + f)

for f in FILES:
    print f
    data = open(f).readlines()
    out = open(f, 'w')
    for line in data:
        for name in CLASSES:
            line = line.replace('eo' + name, name)
        out.write(line)
