var express = require('express');
var router = express.Router();
const path = require('path');
var multer = require('multer');
var fs = require('fs');
var bodyParser  = require('body-parser');

const pathToFile = path.join(__dirname, "detected_hole.ply")
if (fs.existsSync(pathToFile)) {
  try {
    fs.unlinkSync(pathToFile)
    console.log("Successfully deleted the file.")
  } catch(err) {
    throw err
  }
}


router.post('/upload', function(req, res) {
  fs.writeFile('./public/uploads/bun_zipper.ply', res.body, function(err) {
    console.log("upload request sent")
    if (err) {
      res.send('Something when wrong');
    } else {
      res.send('Saved!');
    }
  })
});

// SET STORAGE
var storage = multer.diskStorage({
  destination: function (req, file, cb) {
    cb(null, './public/routes')
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


/* GET home page. */

router.get('/home', function(req, res, next) {
  res.sendFile(path.join(__dirname, '/../../home.html'));
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
  
  var exec = require('child_process').execFileSync;

  var fun =function(){
   console.log("fun() start");
   exec("wine /meshtype.exe bun_zipper.ply")
   //exec(path.join('wine ', __dirname, '/meshtype.exe'), [file.originalname], { cwd: __dirname }).toString()
   //exec(path.join(__dirname, '/conversion_exe.exe'), [file.originalname, 'ply'], { cwd: __dirname }).toString()
  };  

fun(); 
  res.status(204).send(file)


});

router.get('/detectholes', (req, res, next) => {
  console.log("detect holes req sent")

  const filename=req.query["mesh"];
  
  var exec = require('child_process').execFileSync;

  var fun =function(){
  console.log("fun() start")
   console.log(filename);
   exec(path.join('wine ', __dirname, '/hole_detection_executable.exe'), [filename], { cwd: __dirname }).toString()
  };  

fun(); 
  res.status(204).send()


});

module.exports = router;
