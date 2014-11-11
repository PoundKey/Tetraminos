var assert = require("assert");
var expect = require('chai').expect;

describe('Static Parser', function(){

	before(function(){
		//console.log("Get the testing automation started!");
	});

  describe('#indexOf()', function(){
    it('should return -1 when the value is not present', function(){
      assert.equal(-1, [1,2,3].indexOf(5));
      assert.equal(-1, [1,2,3].indexOf(0));
    });

    it('Func should be a string', function(){
    	expect("We we ago!").to.be.a('String');
    });
  });



});