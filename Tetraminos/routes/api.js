var express = require('express');
var router = express.Router();

/* GET users listing. */
router.get('/', function(req, res) {
  res.send('respond with a resource');
});

router.get( '/mario', function (req, res) {

  res.json({title: 'Mario Analyzer', content: 'Dynamic invocation for Mario put here'});

});

router.get( '/pacman', function (req, res) {
  res.json({title: 'Pacman Analyzer', content: 'Dynamic invocation for Pacman put here'});

});

module.exports = router;
