import math
import pyCubbyFlow
import unittest

class QuaternionTest(unittest.TestCase):
	def testInit(self):
		a = pyCubbyFlow.QuaternionD()
		self.assertEqual(a.w, 1.0)
		self.assertEqual(a.x, 0.0)
		self.assertEqual(a.y, 0.0)
		self.assertEqual(a.z, 0.0)
		b = pyCubbyFlow.QuaternionD(-1, 2, 3, 4)
		self.assertEqual(b.w, -1.0)
		self.assertEqual(b.x, 2.0)
		self.assertEqual(b.y, 3.0)
		self.assertEqual(b.z, 4.0)
		c = pyCubbyFlow.QuaternionD(x=2, z=4, w=1, y=3)
		self.assertEqual(c.w, 1.0)
		self.assertEqual(c.x, 2.0)
		self.assertEqual(c.y, 3.0)
		self.assertEqual(c.z, 4.0)

	def testGetters(self):
		a = pyCubbyFlow.QuaternionD(1, 2, 3, 4)
		self.assertEqual(a[0], a.w)
		self.assertEqual(a[1], a.x)
		self.assertEqual(a[2], a.y)
		self.assertEqual(a[3], a.z)
		a.Normalize()
		axis = a.Axis()
		angle = a.Angle()
		denom = math.sqrt(1 - a.w * a.w)
		self.assertAlmostEqual(axis.x, math.sqrt(2.0 / 15.0) / denom)
		self.assertAlmostEqual(axis.y, math.sqrt(3.0 / 10.0) / denom)
		self.assertAlmostEqual(axis.z, 2.0 * math.sqrt(2.0 / 15.0) / denom)
		self.assertAlmostEqual(angle, 2.0 * math.acos(1.0 / math.sqrt(30.0)))

	def testSetters(self):
		a = pyCubbyFlow.QuaternionD()
		a.SetAxisAngle((0, -1, 0), math.pi / 2)
		axis = a.Axis()
		angle = a.Angle()
		self.assertEqual(axis.x, 0)
		self.assertEqual(axis.y, -1)
		self.assertEqual(axis.z, 0)
		self.assertEqual(angle, math.pi / 2)
		a[0] = 1.0; a[1] = 2.0;  a[2] = 3.0;  a[3] = 4.0
		self.assertEqual(1, a.w)
		self.assertEqual(2, a.x)
		self.assertEqual(3, a.y)
		self.assertEqual(4, a.z)

def main():
	pyCubbyFlow.Logging.mute()
	unittest.main()

if __name__ == '__main__':
	main()