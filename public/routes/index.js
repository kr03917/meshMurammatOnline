var express = require('express');
var router = express.Router();
const path = require('path');
var multer = require('multer');

// SET STORAGE
var storage = multer.diskStorage({
  destination: function (req, file, cb) {
    cb(null, './public/uploads')
  },
  filename: function (req, file, cb) {
    cb(null, file.originalname)
  }
})
var upload = multer({ storage: storage })
        

/* GET welcome page. */
router.get('/meshmurammat', function(req, res, next) {
  res.sendFile(path.join(__dirname, '/welcome.html'));
  
});

router.get('/PLYExporter', function(req, res, next) {
  res.sendFile(path.join(__dirname, '/PLYExporter.js'));
  
});

/* GET home page. */

router.get('/home', function(req, res, next) {
  res.sendFile(path.join(__dirname, '/../../home.html'));
  
  var exec = require('child_process').execFile;

  var fun =function(){
   //console.log("fun() start");
   exec(path.join(__dirname, '/mesh.exe'), function(err, data) {  
        
        console.log(err)
        console.log(data.toString());                       
    });  
}
fun();
});

router.post('/uploadfile', upload.single('mesh'), (req, res, next) => {
  console.log("file upload post req sent")
  const file = req.file
  console.log(file)
  if (!file) {
    const error = new Error('Please upload a file')
    error.httpStatusCode = 400
    return next(error)
  }
  res.status(204).send(file)
  var exec = require('child_process').execFile;

  var fun =function(){
   //console.log("fun() start");
   exec(path.join(__dirname, '/my_executable.exe'), function(err, data) {  
        
        console.log(err)
        console.log(data.toString());                       
    });  
}
fun();
});

module.exports = router;
