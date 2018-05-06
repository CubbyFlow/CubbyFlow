import math
import pyCubbyFlow
import unittest

class MyPhysicsAnimation(pyCubbyFlow.PhysicsAnimation):
	def __init__(self):
		self.initData = 0
		self.advData = 0
		super(MyPhysicsAnimation, self).__init__()

	def OnAdvanceTimeStep(self, timeIntervalInSeconds):
		self.advData += 1

	def GetNumberOfSubTimeSteps(self, timeIntervalInSeconds):
		return int(math.ceil(timeIntervalInSeconds / 0.02))

	def OnInitialize(self):
		self.initData = 1

class PhysicsAnimationTests(unittest.TestCase):
	def testInheritance(self):
		anim = MyPhysicsAnimation()

		anim.isUsingFixedSubTimeSteps = False
		f = pyCubbyFlow.Frame(index = 3, timeIntervalInSeconds = 0.1)
		anim.Update(f)
		self.assertEqual(anim.initData, 1)
		self.assertEqual(anim.advData, 20)

def main():
	pyCubbyFlow.Logging.Mute()
	unittest.main()

if __name__ == '__main__':
	main()