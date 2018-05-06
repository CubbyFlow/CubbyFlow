import pyCubbyFlow
import unittest

class MyAnimation(pyCubbyFlow.Animation):
	def __init__(self):
		self.lastFrame = None
		super(MyAnimation, self).__init__()

	def OnUpdate(self, frame):
		self.lastFrame = frame

class AnimationTests(unittest.TestCase):
	def testInheritance(self):
		anim = MyAnimation()
		f = pyCubbyFlow.Frame(3, 0.02)
		anim.Update(f)
		self.assertEqual(anim.lastFrame.index, 3)
		self.assertEqual(anim.lastFrame.timeIntervalInSeconds, 0.02)

def main():
	pyCubbyFlow.Logging.Mute()
	unittest.main()

if __name__ == '__main__':
	main()