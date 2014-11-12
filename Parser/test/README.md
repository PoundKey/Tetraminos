Unit Test for Static Paser
=======
In order to run this unit test, all the nodejs dependencies has to be installed through npm:
var fs = require('fs');
var assert = require("assert");
var expect = require('chai').expect;
var parseString = require('xml2js').parseString;
var readlineSync = require('readline-sync');

Run: mocha parser-test.js

you will be asked to select one of the codebase in oder to proceed the unit test.

The test code is directly injected into the source code the parser, since the parser is written in a functional programming style.

The test code is behavior driven (also known as BDD), which tests all the functionalities, interfaces and outputs match the expected behaviors.

The unit test result for one of the codebase has been attached to the project description readme page.

Cheers.



