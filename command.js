//command line pipe from tap interface to stdio - similar to a vde_plug
var vnic = require("./index.js")(process.argv[2]);
//console.log("Listening to " + process.argv[2]);
//console.log(vnic.uplink);

//process.stdin.pipe(vnic.stream).pipe(process.stdout);
process.stdin.pipe(vnic.uplink);
vnic.downlink.pipe(process.stdout);