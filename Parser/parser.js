/**
 * Created by CTXue on 14-10-17.
 */

var fs = require('fs');
var parseString = require('xml2js').parseString;

var staticInfo = [];

var fileList = fs.readdirSync('./xml');
//console.dir(lists);


var classInfo = function (_class) {

    //information will be used to return a json object from a the translated json file.
    var className = _class.compoundname[0];
    var method = [];
    var field = [];
    var inheritance = [];
    var dependency = [];

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
            return;
        }

    });

    var output = {'className': className, 'method': method, 'field': field, 
                        'inheritance':inheritance, 'dependency': dependency};
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
    //if (file != 'classc_blood.xml') return;
    var path = "./xml/" + file;

    var classFile = fs.readFileSync(path, 'utf8');
    parseString(classFile, function (err, result) {

        if (result.doxygen == undefined) return;
        var obj = result.doxygen.compounddef[0];
        switch (obj.$.kind) {
            case 'class':
                //console.dir("class: " + path);
                classInfo(obj);

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

//console.dir(staticInfo);





