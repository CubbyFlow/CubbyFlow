import numpy as np
import pyCubbyFlow
import unittest

class ParticleSystemData2Tests(unittest.TestCase):
	def testInit(self):
		ps = pyCubbyFlow.ParticleSystemData2()
		self.assertEqual(ps.numberOfParticles, 0)

		ps2 = pyCubbyFlow.ParticleSystemData2(100)
		self.assertEqual(ps2.numberOfParticles, 100)

	def testResize(self):
		ps = pyCubbyFlow.ParticleSystemData2()
		ps.Resize(12)
		self.assertEqual(ps.numberOfParticles, 12)

	def testAddScalarData(self):
		ps = pyCubbyFlow.ParticleSystemData2()
		ps.Resize(12)

		a0 = ps.AddScalarData(2.0)
		a1 = ps.AddScalarData(9.0)
		self.assertEqual(ps.numberOfParticles, 12)
		self.assertEqual(a0, 0)
		self.assertEqual(a1, 1)

		as0 = np.array(ps.ScalarDataAt(a0))
		for val in as0:
			self.assertEqual(val, 2.0)

		as1 = np.array(ps.ScalarDataAt(a1))
		for val in as1:
			self.assertEqual(val, 9.0)

	def testAddVectorData(self):
		ps = pyCubbyFlow.ParticleSystemData2()
		ps.Resize(12)

		a0 = ps.AddVectorData((2.0, 4.0))
		a1 = ps.AddVectorData((9.0, -2.0))
		self.assertEqual(ps.numberOfParticles, 12)
		self.assertEqual(a0, 3)
		self.assertEqual(a1, 4)

		as0 = np.array(ps.VectorDataAt(a0))
		for val in as0:
			self.assertEqual(val.tolist(), [2.0, 4.0])

		as1 = np.array(ps.VectorDataAt(a1))
		for val in as1:
			self.assertEqual(val.tolist(), [9.0, -2.0])

	def testAddParticles(self):
		ps = pyCubbyFlow.ParticleSystemData2()
		ps.Resize(12)

		ps.AddParticles([(1.0, 2.0), (4.0, 5.0)],
						[(7.0, 8.0), (8.0, 7.0)],
						[(5.0, 4.0), (2.0, 1.0)])

		self.assertEqual(ps.numberOfParticles, 14)
		p = np.array(ps.positions)
		v = np.array(ps.velocities)
		f = np.array(ps.forces)

		self.assertEqual([1.0, 2.0], p[12].tolist())
		self.assertEqual([4.0, 5.0], p[13].tolist())
		self.assertEqual([7.0, 8.0], v[12].tolist())
		self.assertEqual([8.0, 7.0], v[13].tolist())
		self.assertEqual([5.0, 4.0], f[12].tolist())
		self.assertEqual([2.0, 1.0], f[13].tolist())

class ParticleSystemData3Tests(unittest.TestCase):
	def testInit(self):
		ps = pyCubbyFlow.ParticleSystemData3()
		self.assertEqual(ps.numberOfParticles, 0)

		ps2 = pyCubbyFlow.ParticleSystemData3(100)
		self.assertEqual(ps2.numberOfParticles, 100)

	def testResize(self):
		ps = pyCubbyFlow.ParticleSystemData3()
		ps.Resize(12)
		self.assertEqual(ps.numberOfParticles, 12)

	def testAddScalarData(self):
		ps = pyCubbyFlow.ParticleSystemData3()
		ps.Resize(12)

		a0 = ps.AddScalarData(2.0)
		a1 = ps.AddScalarData(9.0)
		self.assertEqual(ps.numberOfParticles, 12)
		self.assertEqual(a0, 0)
		self.assertEqual(a1, 1)

		as0 = np.array(ps.ScalarDataAt(a0))
		for val in as0:
			self.assertEqual(val, 2.0)

		as1 = np.array(ps.ScalarDataAt(a1))
		for val in as1:
			self.assertEqual(val, 9.0)

	def testAddVectorData(self):
		ps = pyCubbyFlow.ParticleSystemData3()
		ps.Resize(12)

		a0 = ps.AddVectorData((2.0, 4.0, -1.0))
		a1 = ps.AddVectorData((9.0, -2.0, 5.0))
		self.assertEqual(ps.numberOfParticles, 12)
		self.assertEqual(a0, 3)
		self.assertEqual(a1, 4)

		as0 = np.array(ps.VectorDataAt(a0))
		for val in as0:
			self.assertEqual(val.tolist(), [2.0, 4.0, -1.0])

		as1 = np.array(ps.VectorDataAt(a1))
		for val in as1:
			self.assertEqual(val.tolist(), [9.0, -2.0, 5.0])

	def testAddParticles(self):
		ps = pyCubbyFlow.ParticleSystemData3()
		ps.Resize(12)

		ps.AddParticles([(1.0, 2.0, 3.0), (4.0, 5.0, 6.0)],
						[(7.0, 8.0, 9.0), (8.0, 7.0, 6.0)],
						[(5.0, 4.0, 3.0), (2.0, 1.0, 3.0)])

		self.assertEqual(ps.numberOfParticles, 14)
		p = np.array(ps.positions)
		v = np.array(ps.velocities)
		f = np.array(ps.forces)

		self.assertEqual([1.0, 2.0, 3.0], p[12].tolist())
		self.assertEqual([4.0, 5.0, 6.0], p[13].tolist())
		self.assertEqual([7.0, 8.0, 9.0], v[12].tolist())
		self.assertEqual([8.0, 7.0, 6.0], v[13].tolist())
		self.assertEqual([5.0, 4.0, 3.0], f[12].tolist())
		self.assertEqual([2.0, 1.0, 3.0], f[13].tolist())

def main():
	pyCubbyFlow.Logging.mute()
	unittest.main()

if __name__ == '__main__':
	main()