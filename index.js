var tuntap = require('./build/Release/tuntap');
var fs = require('fs');
//Simple interface takes single string parameter tapNN or tunNN, where NN is a number
//TODO - if NN is ommitted the next available interface ID will be used
//TODO - add options
var Interface = function(name) {
    var fd;
    if(name.indexOf("tap"==0)){
        fd = tuntap.addTap(name);
    }
    else if(name.indexOf("tun"==0)){
        fd = tuntap.addTun(name);
    }
    //TODO if NN ommitted, use next available and return new name
   
    var sR = fs.createReadStream(null, {fd: fd, highWaterMark: Math.pow(2,16)});
    var sW = fs.createWriteStream(null, {fd: fd, highWaterMark: Math.pow(2,16)});
    
    return{
        name:name,
        fileDescriptor:fd,
        uplink:sW,
        downlink:sR
}};

module.exports = function(name){
    //todo - if tap or tun, use right mthod
    //todo add error checking, permissions, etc
    return new Interface(name);
}
//console.log(tuntap.addTap("tap0"));