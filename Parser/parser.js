/**
 * Created by CTXue on 14-10-17.
 */

var fs = require('fs');
var parseString = require('xml2js').parseString;
var readline = require('readline');
var inputCodeBase = './xml_';

/**
var prompts = readline.createInterface(process.stdin, process.stdout);

prompts.question("Please enter a codebase (mario || pacman): ", function(answer) {
	inputCodeBase = inputCodeBase + answer;
  console.log("Thank you for your valuable feedback: ", answer);
  prompts.close();
});
*/

var staticInfo = [];
var writeOutput;
var fileList = fs.readdirSync('./xml');
//console.dir(lists);

var classInfo = function (_class) {

    //information will be used to return a json object from a the translated json file.
    var className = _class.compoundname[0];
    var method = [];
    var field = [];
    var inheritance;
    var dependency = [];
    var output;

    //console.log(JSON.stringify(_class, null, 4));
    _class.basecompoundref ? inheritance = _class.basecompoundref[0]._ : inheritance=null;
    //entityArray, length in 2 or 1, each contains set of attributes or methods.
    var entityArray = _class.sectiondef;

    entityArray.forEach(function (entity) {

        //type of the entity: attrib vs func
        var entityType = entity.$.kind;
        // member definition [Array] inside of the entity
        var memDef = entity.memberdef;
        // console.dir(entity);
        if (entityType.search('attrib') != -1) {
            //loop through each attribute inside of member definition array.
            memDef.forEach(function (attr) {

                //double check the attribute is a field variable
                var typeCheck = attr.$.kind;
                if (typeCheck != 'variable') return;

                field.push(attr.name[0]);
                //console.dir(field);
            });


        } else if (entityType.search('func') != -1) {
            memDef.forEach(function (func) {
                //console.dir(func);
                var typeCheck = func.$.kind;
                if (typeCheck != 'function') return;
                method.push(func.name[0]);

            });


        } else {
            console.log('Entity has not been defined yet');
        }

    });

    output = {
        'className': className, 'method': method, 'field': field,
        'inheritance': inheritance, 'dependency': dependency
    };

    //return the parsed info for single xml file as the output
    return output;


};

var fileInfo = function (_file) {
    //console.dir(_file.$);

};

var structInfo = function (_struct) {
    //console.dir(_struct.$);

};


fileList.forEach(function (file) {

    if (file.search('.xml') == -1) return;
    //if (file != 'classc_spiny.xml') return;
    var path = "./xml/" + file;

    var classFile = fs.readFileSync(path, 'utf8');
    parseString(classFile, function (err, result) {

        if (result.doxygen == undefined) return;
        var obj = result.doxygen.compounddef[0];
        switch (obj.$.kind) {
            case 'class':
                //console.dir("class: " + path);
                var output = classInfo(obj);
                staticInfo.push(output);
                break;

            case 'file':
                //console.dir("file: " + path);
                fileInfo(obj);
                break;

            case 'struct':
                //console.dir("struct: " + path);
                structInfo(obj);
                break;

            default :
                console.dir("unknown: " + path);
                break;
        }


    });

});



writeOutput = {'staticInfo': staticInfo};
//console.dir(writeOutput);
//console.dir(JSON.stringify(writeOutput, null, 4));
var outputFilename = 'static_info.json';
fs.writeFile(outputFilename, JSON.stringify(writeOutput, null, 4), 'utf8', function (err) {

    err ? console.dir(err) : console.dir("Static info successfully saved to: " + outputFilename);

});


