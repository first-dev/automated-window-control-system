#!/usr/bin/env python3
import unittest
import os
import subprocess
import Adafruit_DHT


c_test_files = ['read']
gcc_args = '/home/moham/drivers/io/*.* /home/moham/libs/timing/*.*'


class MyTestResult(unittest.TestResult):
  def addError(self, test, err):
    # Override the addError method to hide the traceback
    self.errors.append((test, "ERROR: {}".format(err[1])))


class Test(unittest.TestCase):
  def setUp(self):
    for test_file in c_test_files:
      os.system(f'gcc -o {test_file} {test_file}.c  ../*.c ../*.h {gcc_args}')
      self.sensor = Adafruit_DHT.DHT11
      self.pin = 14

  def test_read(self):
    result = subprocess.run(
        ['./read', str(self.pin)], stdout=subprocess.PIPE)
    output = result.stdout
    output = output.decode('utf-8')
    if output.startswith('error'):
      error_code = int(output.split(' ')[1])
      if (error_code == -1):
        self.fail('Error: reponse signal too short')
      elif (error_code == -2):
        self.fail('Error: checksum error')
    result_humidity = output.split(' ')[0]
    result_temperature = output.split(' ')[1]
    humidity, temperature = Adafruit_DHT.read(self.sensor, self.pin)
    self.assertEqual(float(result_humidity), float(humidity))
    self.assertEqual(float(result_temperature), float(temperature))

  def tearDown(self):
    for test_file in c_test_files:
      os.system(f'rm {test_file}')


if __name__ == '__main__':
  unittest.main()
