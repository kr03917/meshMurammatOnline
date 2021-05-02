/*Script for .obj file load on browser*/
/*Import three.js modules*/

import * as THREE from 'https://cdn.jsdelivr.net/npm/three@0.118.3/build/three.module.js';
import { OBJLoader } from 'https://cdn.jsdelivr.net/npm/three@0.118.3/examples/jsm/loaders/OBJLoader.js';
import {OrbitControls} from 'https://cdn.jsdelivr.net/npm/three@0.118.3/examples/jsm/controls/OrbitControls.js';
	
/***Main Code***/

var input = document.getElementById( 'file-input' ); 

/*Get file input from HTML file upload button*/	
input.addEventListener( 'change', function( event ) {
	
	var file = this.files[0];					//Get the input file
	var reader = new FileReader();				//Asynchronously read file content	
	
	/*Listener is used to wait until file is loaded*/
	reader.addEventListener( 'load', function (event) {
		var contents = event.target.result;				
		var mesh = new THREE.PLYLoader().parse(contents);		//Parse the .obj file content to get mesh
		scene.add(mesh);
		console.log("file loaded")
	}, false );
	
	/*Error check*/
	if ( reader.readAsBinaryString !== undefined ) {
		reader.readAsBinaryString( file );
	} else {
		reader.readAsArrayBuffer( file );
	}
});


//Create scene and add camera
const scene = new THREE.Scene();
scene.background = new THREE.Color(0x111111);
const camera = new THREE.PerspectiveCamera( 75, window.innerWidth / window.innerHeight, 0.1, 1000 );
camera.position.set(0,1,-10);
				
//Add lights				
var light = new THREE.PointLight(0xffffff, 1, 0);
light.position.set(50,50,50);
scene.add(light);
var ambientLight = new THREE.AmbientLight(0x404040);
scene.add(ambientLight);

//Create renderer
const renderer = new THREE.WebGLRenderer();
renderer.setSize( window.innerWidth, window.innerHeight );
document.body.appendChild( renderer.domElement );
			
//Add controls to zoom in and out
var controls = new OrbitControls(camera, renderer.domElement);
controls.zoomSpeed = 0.4;
controls.panSpeed = 0.4;
	
//Render function	
function render() {
	requestAnimationFrame( render );
	renderer.render( scene, camera );
};

//render the scene
render();
