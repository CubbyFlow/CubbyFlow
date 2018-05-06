import pyCubbyFlow
import unittest

class FLIPSolver3Tests(unittest.TestCase):
	def testInit(self):
		a = pyCubbyFlow.FLIPSolver3()
		self.assertEqual(a.resolution, (1, 1, 1))
		self.assertEqual(a.gridSpacing, (1.0, 1.0, 1.0))
		self.assertEqual(a.gridOrigin, (0.0, 0.0, 0.0))

		b = pyCubbyFlow.FLIPSolver3((2, 3, 4), (5, 6, 7), (8, 9, 10))
		self.assertEqual(b.resolution, (2, 3, 4))
		self.assertEqual(b.gridSpacing, (5.0, 6.0, 7.0))
		self.assertEqual(b.gridOrigin, (8.0, 9.0, 10.0))

		c = pyCubbyFlow.FLIPSolver3(resolution=(2, 3, 4), gridSpacing=(5, 6, 7), gridOrigin=(8, 9, 10))
		self.assertEqual(c.resolution, (2, 3, 4))
		self.assertEqual(c.gridSpacing, (5.0, 6.0, 7.0))
		self.assertEqual(c.gridOrigin, (8.0, 9.0, 10.0))

	def testAnimation(self):
		a = pyCubbyFlow.FLIPSolver3()
		f = pyCubbyFlow.Frame()
		a.Update(f)
		self.assertEqual(a.currentFrame.index, 0)
		f.Advance()
		a.Update(f)
		self.assertEqual(a.currentFrame.index, 1)

	def testPhysicsAnimation(self):
		a = pyCubbyFlow.FLIPSolver3()
		a.isUsingFixedSubTimeSteps = False
		self.assertFalse(a.isUsingFixedSubTimeSteps)
		a.isUsingFixedSubTimeSteps = True
		self.assertTrue(a.isUsingFixedSubTimeSteps)
		a.numberOfFixedSubTimeSteps = 42
		self.assertEqual(a.numberOfFixedSubTimeSteps, 42)
		a.AdvanceSingleFrame()
		self.assertEqual(a.currentFrame.index, 0)
		self.assertAlmostEqual(a.currentTimeInSeconds, 0.0, delta=1e-12)
		a.AdvanceSingleFrame()
		self.assertEqual(a.currentFrame.index, 1)
		self.assertAlmostEqual(a.currentTimeInSeconds, 1.0 / 60.0, delta=1e-12)

	def testGridFluidSolver3(self):
		a = pyCubbyFlow.FLIPSolver3()
		a.gravity = (1.0, 2.0, 3.0)
		self.assertEqual(a.gravity, (1.0, 2.0, 3.0))
		a.viscosityCoefficient = 0.042
		self.assertEqual(a.viscosityCoefficient, 0.042)
		a.maxCFL = 3.0
		self.assertEqual(a.maxCFL, 3.0)
		a.ResizeGrid((2, 3, 4), (5, 6, 7), (8, 9, 10))
		self.assertEqual(a.resolution, (2, 3, 4))
		self.assertEqual(a.gridSpacing, (5.0, 6.0, 7.0))
		self.assertEqual(a.gridOrigin, (8.0, 9.0, 10.0))
		ds = pyCubbyFlow.GridForwardEulerDiffusionSolver3()
		a.diffusionSolver = ds
		self.assertEqual(ds, a.diffusionSolver)

	def testFlipSolver3(self):
		a = pyCubbyFlow.FLIPSolver3()
		a.picBlendingFactor = 0.7
		self.assertEqual(a.picBlendingFactor, 0.7)

def main():
	pyCubbyFlow.Logging.mute()
	unittest.main()

if __name__ == '__main__':
	main()