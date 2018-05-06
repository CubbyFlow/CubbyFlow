import pyCubbyFlow
import unittest

class Sphere3Test(unittest.TestCase):
    def testInit(self):
        a = pyCubbyFlow.Sphere3()
        self.assertEqual(a.isNormalFlipped, False)
        self.assertEqual(a.center, (0, 0, 0))
        self.assertEqual(a.radius, 1.0)

def main():
    pyCubbyFlow.Logging.Mute()
    unittest.main()

if __name__ == '__main__':
    main()