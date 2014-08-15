{
  "targets": [
    {
      "target_name": "tuntap",
      "sources": [ "src/tuntap.cc" ],
      "include_dirs" : [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
  
}