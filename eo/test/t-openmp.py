#!/usr/bin/env python

import pylab
import optparse, logging, sys, os
from datetime import datetime

LEVELS = {'debug': logging.DEBUG,
          'info': logging.INFO,
          'warning': logging.WARNING,
          'error': logging.ERROR,
          'critical': logging.CRITICAL}

LOG_DEFAULT_FILENAME='notitle.log'

OPENMP_EXEC_FORMAT='./test/t-openmp -p=%d --popStep=%d -P=%d -d=%d --dimStep=%d -D=%d -r=%d --seed=%d -v=%s -H=%s'

RESULT_FILE_FORMAT='%s%s_p%d_pS%d_P%d_d%d_dS%d_D%d_r%d_s%d'

def parser(parser=optparse.OptionParser()):
    # general parameters
    parser.add_option('-v', '--verbose', choices=LEVELS.keys(), default='info', help='set a verbose level')
    parser.add_option('-f', '--file', help='give an input project filename', default='')
    parser.add_option('-o', '--output', help='give an output filename for logging', default=LOG_DEFAULT_FILENAME)
    # general parameters ends

    parser.add_option('-r', '--nRun', default=100, help='how many times you would compute each iteration ?')
    parser.add_option('-s', '--seed', default=1, help='give here a seed value')

    topic = str(datetime.today())
    for char in [' ', ':', '-', '.']: topic = topic.replace(char, '_')
    parser.add_option('-t', '--topic', default='openmp_measures_' + topic + '/', help='give here a topic name used to create the folder')

    parser.add_option('-E', '--onlyexecute', action='store_true', dest='onlyexecute', default=False, help='used this option if you only want to execute measures without generating images')
    parser.add_option('-X', '--onlyprint', action='store_true', dest='onlyprint', default=False, help='used this option if you only want to generate images without executing measures, dont forget to set the good path in using --topic with a "/" at the end')

    options, args = parser.parse_args()

    logger(options.verbose, options.output)

    return options

def logger(level_name, filename=LOG_DEFAULT_FILENAME):
    logging.basicConfig(
        level=logging.DEBUG,
        format='%(asctime)s %(name)-12s %(levelname)-8s %(message)s',
        filename=filename, filemode='a'
        )

    console = logging.StreamHandler()
    console.setLevel(LEVELS.get(level_name, logging.NOTSET))
    console.setFormatter(logging.Formatter('%(name)-12s: %(levelname)-8s %(message)s'))
    logging.getLogger('').addHandler(console)

options = parser()

def get_boxplot_data( filename ):
    try:
        f = open( filename )
        return [ [ float(value) for value in line.split() ] for line in f.readlines() ]
    except:
        raise ValueError('got an issue during the reading of file %s' % filename)

def non_zero( value ): return value if value > 0 else 1

def do_measure( name, p, ps, P, d, ds, D, r=options.nRun, s=options.seed, v='logging' ):
    pwd = options.topic + name + '_'
    cmd = OPENMP_EXEC_FORMAT % (p, ps, P, d, ds, D, r, s, v, pwd)
    logging.debug( cmd )
    if not options.onlyprint:
        os.system( cmd )

    if not options.onlyexecute:
        for cur in ['speedup', 'efficiency', 'dynamicity']:
            filename = RESULT_FILE_FORMAT % (pwd, cur, p, ps, P, d, ds, D, r, s)
            pylab.boxplot( get_boxplot_data( filename ) )
            iters = ( non_zero( P - p ) / ps ) * ( non_zero( D - d ) / ds )
            pylab.xlabel('%d iterations from %d,%d to %d,%d' % ( iters, p, d, P, D) )
            pylab.ylabel('%s - %s' % (cur, name))
            pylab.savefig( filename + '.pdf', format='pdf' )
            pylab.savefig( filename + '.png', format='png' )
            pylab.cla()
            pylab.clf()

def main():
    if not options.onlyprint:
        logging.info('creates first the new topic repository %s', options.topic)
        os.mkdir( options.topic )

    logging.info('do all tests with r = %d and a common seed value = %d' % (options.nRun, options.seed))

    logging.info('EA in time O(1) and O(n) - speedup measure Sp, Ep and Dp for P & D')

    logging.info('(1) measure for all combinaisons of P n D')
    do_measure( '1', 1, 10, 101, 1, 10, 101 )

    logging.info('(2) measure for P \in [1, 100[ with D fixed to 1000')
    do_measure( '2', 1, 1, 101, 1000, 1, 1000 )

    logging.info('(3) measure for P \in [1, 1000[ with ps = 10 and D fixed to 1000')
    do_measure( '3', 1, 10, 1001, 1000, 1, 1000 )

    logging.info('(4) measure for D \in [1, 100[ with P fixed to 1000')
    do_measure( '4', 1000, 1, 1000, 1, 1, 101 )

    logging.info('(5) measure for D \in [1, 1000[ with ds = 10 and P fixed to 1000')
    do_measure( '5', 1000, 1, 1000, 1, 10, 1001 )

# when executed, just run main():
if __name__ == '__main__':
    logging.debug('### plotting started ###')

    main()

    logging.debug('### plotting ended ###')