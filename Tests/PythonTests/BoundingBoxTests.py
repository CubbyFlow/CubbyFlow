import pyCubbyFlow
import unittest

class BoundingBox2FTests(unittest.TestCase):
	def testInit(self):
		a = pyCubbyFlow.BoundingBox2F()
		self.assertGreater(a.lowerCorner.x, a.upperCorner.x)
		self.assertGreater(a.lowerCorner.y, a.upperCorner.y)
		b = pyCubbyFlow.BoundingBox2F((-1, -2), (4, 2))
		self.assertEqual(b.lowerCorner.x, -1.0)
		self.assertEqual(b.lowerCorner.y, -2.0)
		self.assertEqual(b.upperCorner.x, 4.0)
		self.assertEqual(b.upperCorner.y, 2.0)
		l, c = pyCubbyFlow.Vector2F(-1, -2), pyCubbyFlow.Vector2F(4, 2)
		c = pyCubbyFlow.BoundingBox2F(l, c)
		self.assertEqual(c.lowerCorner.x, -1.0)
		self.assertEqual(c.lowerCorner.y, -2.0)
		self.assertEqual(c.upperCorner.x, 4.0)
		self.assertEqual(c.upperCorner.y, 2.0)

	def testIsEmpty(self):
		a = pyCubbyFlow.BoundingBox2F((-2.0, -2.0), (4.0, 3.0))
		self.assertFalse(a.IsEmpty())


class BoundingBox2DTests(unittest.TestCase):
	def testInit(self):
		a = pyCubbyFlow.BoundingBox2D()
		self.assertGreater(a.lowerCorner.x, a.upperCorner.x)
		self.assertGreater(a.lowerCorner.y, a.upperCorner.y)
		b = pyCubbyFlow.BoundingBox2D((-1, -2), (4, 2))
		self.assertEqual(b.lowerCorner.x, -1.0)
		self.assertEqual(b.lowerCorner.y, -2.0)
		self.assertEqual(b.upperCorner.x, 4.0)
		self.assertEqual(b.upperCorner.y, 2.0)
		l, u = pyCubbyFlow.Vector2D(-1, -2), pyCubbyFlow.Vector2D(4, 2)
		c = pyCubbyFlow.BoundingBox2D(l, u)
		self.assertEqual(c.lowerCorner.x, -1.0)
		self.assertEqual(c.lowerCorner.y, -2.0)
		self.assertEqual(c.upperCorner.x, 4.0)
		self.assertEqual(c.upperCorner.y, 2.0)

	def testIsEmpty(self):
		a = pyCubbyFlow.BoundingBox2D((-2.0, -2.0), (4.0, 3.0))
		self.assertFalse(a.IsEmpty())


class BoundingBox3FTests(unittest.TestCase):
	def testInit(self):
		a = pyCubbyFlow.BoundingBox3F()
		self.assertGreater(a.lowerCorner.x, a.upperCorner.x)
		self.assertGreater(a.lowerCorner.y, a.upperCorner.y)
		self.assertGreater(a.lowerCorner.z, a.upperCorner.z)
		b = pyCubbyFlow.BoundingBox3F((-1, -2, -3), (4, 2, 5))
		self.assertEqual(b.lowerCorner.x, -1.0)
		self.assertEqual(b.lowerCorner.y, -2.0)
		self.assertEqual(b.lowerCorner.z, -3.0)
		self.assertEqual(b.upperCorner.x, 4.0)
		self.assertEqual(b.upperCorner.y, 2.0)
		self.assertEqual(b.upperCorner.z, 5.0)
		l, c = pyCubbyFlow.Vector3F(-1, -2, -3), pyCubbyFlow.Vector3F(4, 2, 5)
		c = pyCubbyFlow.BoundingBox3F(l, c)
		self.assertEqual(c.lowerCorner.x, -1.0)
		self.assertEqual(c.lowerCorner.y, -2.0)
		self.assertEqual(c.lowerCorner.z, -3.0)
		self.assertEqual(c.upperCorner.x, 4.0)
		self.assertEqual(c.upperCorner.y, 2.0)
		self.assertEqual(c.upperCorner.z, 5.0)

	def testIsEmpty(self):
		a = pyCubbyFlow.BoundingBox3F((-2.0, -2.0, 1.0), (4.0, 3.0, 5.0))
		self.assertFalse(a.IsEmpty())


class BoundingBox3DTests(unittest.TestCase):
	def testInit(self):
		a = pyCubbyFlow.BoundingBox3D()
		self.assertGreater(a.lowerCorner.x, a.upperCorner.x)
		self.assertGreater(a.lowerCorner.y, a.upperCorner.y)
		self.assertGreater(a.lowerCorner.z, a.upperCorner.z)
		b = pyCubbyFlow.BoundingBox3D((-1, -2, -3), (4, 2, 5))
		self.assertEqual(b.lowerCorner.x, -1.0)
		self.assertEqual(b.lowerCorner.y, -2.0)
		self.assertEqual(b.lowerCorner.z, -3.0)
		self.assertEqual(b.upperCorner.x, 4.0)
		self.assertEqual(b.upperCorner.y, 2.0)
		self.assertEqual(b.upperCorner.z, 5.0)
		l, c = pyCubbyFlow.Vector3D(-1, -2, -3), pyCubbyFlow.Vector3D(4, 2, 5)
		c = pyCubbyFlow.BoundingBox3D(l, c)
		self.assertEqual(c.lowerCorner.x, -1.0)
		self.assertEqual(c.lowerCorner.y, -2.0)
		self.assertEqual(c.lowerCorner.z, -3.0)
		self.assertEqual(c.upperCorner.x, 4.0)
		self.assertEqual(c.upperCorner.y, 2.0)
		self.assertEqual(c.upperCorner.z, 5.0)

	def testIsEmpty(self):
		a = pyCubbyFlow.BoundingBox3D((-2.0, -2.0, 1.0), (4.0, 3.0, 5.0))
		self.assertFalse(a.IsEmpty())

def main():
	pyCubbyFlow.Logging.mute()
	unittest.main()

if __name__ == '__main__':
	main()