import pyCubbyFlow
import unittest

class FaceCenteredGrid2Tests(unittest.TestCase):
	def testFill(self):
		a = pyCubbyFlow.FaceCenteredGrid2((10, 10))
		a.Fill((3.0, 4.0))
		for j in range(10):
			for i in range(11):
				self.assertEqual(a.GetU(i, j), 3.0)
		for j in range(11):
			for i in range(10):
				self.assertEqual(a.GetV(i, j), 4.0)

		def filler(pt):
			return (pt.x, pt.y)

		a.Fill(filler)
		for j in range(10):
			for i in range(11):
				self.assertEqual(a.GetU(i, j), i)
		for j in range(11):
			for i in range(10):
				self.assertEqual(a.GetV(i, j), j)

	def testForEach(self):
		a = pyCubbyFlow.FaceCenteredGrid2((10, 10))
		d = {'ei': 0, 'ej': 0}

		def checkU(i, j):
			self.assertEqual(i, d['ei'])
			self.assertEqual(j, d['ej'])
			d['ei'] += 1
			if d['ei'] >= 11:
				d['ei'] = 0
				d['ej'] += 1

		a.ForEachUIndex(checkU)
		d = {'ei': 0, 'ej': 0}

		def checkV(i, j):
			self.assertEqual(i, d['ei'])
			self.assertEqual(j, d['ej'])
			d['ei'] += 1
			if d['ei'] >= 10:
				d['ei'] = 0
				d['ej'] += 1

		a.ForEachVIndex(checkV)

	def testSerialization(self):
		a = pyCubbyFlow.FaceCenteredGrid2((10, 10))

		def filler(pt):
			return (pt.x, pt.y)

		a.Fill(filler)

		flatBuffer = a.Serialize()

		b = pyCubbyFlow.FaceCenteredGrid2()
		b.Deserialize(flatBuffer)

		for j in range(10):
			for i in range(11):
				self.assertEqual(b.GetU(i, j), i)
		for j in range(11):
			for i in range(10):
				self.assertEqual(b.GetV(i, j), j)

def main():
	pyCubbyFlow.Logging.mute()
	unittest.main()

if __name__ == '__main__':
	main()