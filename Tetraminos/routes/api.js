var express = require('express');
var router = express.Router();
var sys = require('sys');
var exec = require('child_process').exec;
var child;

/* GET users listing. */
router.get('/', function (req, res) {
    res.send('respond with a resource');
});

router.get('/mario', function (req, res) {
    //if (process.getuid) {
    //    console.log('Current uid: ' + process.getuid());
    //}
    var output = undefined;
    child = exec("./mario.out", {cwd: "static_lib"}, function (error, stdout, stderr) {
        //sys.print('stdout: ' + stdout);
        //sys.print('stderr: ' + stderr);
        //output = stdout.split("=");
        console.log(stdout);
        if (error !== null) {
            console.log('exec error: ' + error);
        }
    });


    res.json({title: 'Mario Analyzer', content: 'Checkout the console for stdout.' });

});

router.get('/pacman', function (req, res) {
    res.json({title: 'Pacman Analyzer', content: 'Dynamic invocation for Pacman put here'});

});

module.exports = router;
