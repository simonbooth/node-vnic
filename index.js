var tuntap = require('./build/Release/tuntap');
var fs = require('fs');

var Interface = function(name) {
    var fd = tuntap.addTap(name);
    var sR = fs.createReadStream(null, {fd: fd});
    var sW = fs.createReadStream(null, {fd: fd});
    return{
    uplink:sW,
    downlink:sR
}};

module.exports = function(name){
    //todo - if tap or tun, use right mthod
    //todo add error checking, permissions, etc
    return new Interface(name);
}
//console.log(tuntap.addTap("tap0"));