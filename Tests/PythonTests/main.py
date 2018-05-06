import pyCubbyFlow
import unittest

from AnimationTests import *
from BoundingBoxTests import *
from FaceCenteredGridTests import *
from FLIPSolverTests import *
from ParticleSystemDataTests import *
from PhysicsAnimationTests import *
from SPHSystemDataTests import *
from SphereTests import *
from VectorTests import *
from QuaternionTests import *

def main():
	pyCubbyFlow.Logging.Mute()
	unittest.main()

if __name__ == '__main__':
	main()