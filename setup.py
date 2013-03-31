#!/usr/bin/env python

from setuptools import setup, find_packages

setup(
	name="tm1640",
	version="0.1",
	description="Library to allow Raspberry Pi to write to TM1640 display",
	author="Michael Farrell",
	author_email="micolous@gmail.com",
	url="",
	package_dir={'':'src/python'},
	py_modules=['tm1640'],
)
