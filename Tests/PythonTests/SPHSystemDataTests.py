import pyCubbyFlow
import unittest

class SPHSystemData2Tests(unittest.TestCase):
	def testParameters(self):
		data = pyCubbyFlow.SPHSystemData2()

		data.targetDensity = 123.0
		data.targetSpacing = 0.549
		data.relativeKernelRadius = 2.5

		self.assertEqual(data.targetDensity, 123.0)
		self.assertEqual(data.targetSpacing, 0.549)
		self.assertEqual(data.radius, 0.549)
		self.assertEqual(data.relativeKernelRadius, 2.5)
		self.assertEqual(data.kernelRadius, 2.5 * 0.549)

		data.kernelRadius = 1.9
		self.assertEqual(data.kernelRadius, 1.9)
		self.assertEqual(data.targetSpacing, 1.9 / 2.5)

		data.radius = 0.413
		self.assertEqual(data.targetSpacing, 0.413)
		self.assertEqual(data.radius, 0.413)
		self.assertEqual(data.relativeKernelRadius, 2.5)
		self.assertEqual(data.kernelRadius, 2.5 * 0.413)

		data.mass = 2.0 * data.mass
		self.assertEqual(data.targetDensity, 246.0)

def main():
	pyCubbyFlow.Logging.mute()
	unittest.main()

if __name__ == '__main__':
	main()