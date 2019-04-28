This documentation presents the results and implementation details of a method for scanning a surface's Bi-Directional Reflectance Distribution Function, or short BRDF. This method was developed by Christian Thurow and Christopher Leiste
as a project within one semester as part of the course "Rapid Prototyping" (winter term 2011/12) at <a href="http://www.cg.tu-berlin.de/">Technical University Berlin</a>.
      
Our main goal was to aquire surface properties regarding its light reflectance of real objects as accurate as possible given the limited equipment of an off-the-shelve webcam and a <a href="http://www.david-laserscanner.com/">DAVID Laserscanner</a>. Together with a 3D model of the object, the BRDF description of its surface reassembles a real object quite well and makes it possible to render this               object realistically and also manufacture replicas of the object with modern rapid prototyping 3D printers. But before we go into detail of our implementation, we have to have a look at the BRD function, briefly its theory and complexity to fully understand its basic idea and importance.

<img src="Website/img/reflection_t.png" width="350" />

Figure 2: Geometry of Reflection

The BRDF describes how incident light is reflected on a surface. The amount and direction of the reflected light
depends on the surface normal vector-N, the direction of the incident light vector-L, the position of the spectator(vector-V),
and material specific coefficients that are the unknowns in our estimation process. The coefficients tell us about the amount of diffuse light (k<sub>d</sub>), specular light (k<sub>s</sub>) and the specular hardness(n), respectively. In the field of Computer Graphics, there are several different established reflectance models. Among others, two very common models are the <a href="http://www.cs.northwestern.edu/~ago820/cs395/Papers/Phong_1975.pdf">Phong reflection model</a> and the
<a href="http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.131.7741&rep=rep1&type=pdf">Blinn-Phong model</a>, which are described by the following formulas.

<img src="Website/img/formulas_t.png" width="600" />

Where H is the half-vector between V and L, and R is the mirrored version of L at N.
Figure 2 shows the angular relations of the reflectance problem. The BRDF Estimation problem has multiple dimensions making
this a complex task. Besides the 3-dimensional angular relations, involving the state of the 3 vectors N,L and V, each point of
the surface can have different reflectance properties, also each wavelength of light may be reflected differently.
However, once a full BRDF formula for each wavelength and each point on the surface has been acquired, the surface properties are fully
described (except for subsurface scattering) in the sense of light interaction, leading to many new possible applications.

Implementation:

In order to build a BRDF Scanner, several components are needed. What we alredy had access to is a DAVID Laserscanner, constisting of a reference pattern, a webcam and a line laser. This gives us a 3D representation in form of an obj file. What is missing is a moveable light source in order to create different lighting conditions, covering a wide range of possible values in the BRD equation. We chose to integrate the light source into a scanner skeleton, which is described in detail in the Implementation – Hardware section. Once measurements are taken, we have discrete values for certain angles, so we need a software that calculates and may render a full BRDF. The latter is described in the Implementation – Software section.

Hardware:

Meaningful measurements for BRDF estimation can be acquired by taking pictures of the scene, having the camera at a static position, but moving a point light source to predefined spots. This basic idea is demonstrated in Figure 1. We chose to go with multiple light sources turned on one at a time rather than a moveable light source for the sake of much easier implementation. For this we use a 4x4 matrix of super bright white LEDs with a wide angle of radiation which are aligned concentrically around the centre of the scene in different heights. This makes it easier later in the estimation process, because the positions of the discrete light sources are known and can be hard-coded. The following images show photos taken during the building process.

<a href="Website/img/IMG_0904.JPG"><img src="Website/img/IMG_0904_t.JPG" width="475" /></a>
Figure 2: Building the Skeleton 1

<a href="Website/img/IMG_0906.JPG"><img src="Website/img/IMG_0906_t.JPG" width="475" /></a>
Figure 3: Building the Skeleton 2
				
We built the frame out of 0.8cm scantling timber, 0.5cm for the cross beams respectively. For the bended beams we used a laser cutter to cut thin layers of artificial wood, since these layers are too thin to hold their own weight over the length of more than 30cm, we glued 3 of them together to create a very solid structure. The template for the bended beams can be downloaded from this page in the <a href="download.html">Downloads</a> section. The template also already includes slots for the LED legs. Only a fraction of them is used by our 16 leds, this however makes the frame easily extensible. All beams are glued together making it a very rigid structure. The frames dimensions are built so that the calibration scheme of the laser scanner slots perfectly into place.

<a href="Website/img/skeleton.JPG"><img src="Website/img/skeleton_t.jpg" width="475" /></a>
Figure 4: Finished Skeleton
               
<a href="Website/img/IMG_0910.JPG"><img src="Website/img/IMG_0910_t.JPG" width="475" /></a>
Figure 5: Finished Skeleton backside
				
At this point, the skeleton is finished, but the structure is still missing the point light sources. LEDs are slot into the perforated bended beams, with the LED's pointing inwards. The wiring of the LED matrix is implemented so that only one LED may be lit at a time. For controlling the LEDs we use a standard microcontroller. We chose the <a href="http://www.arduino.cc/">Arduino</a> Uno for its many output ports as well as ease of programmability and low asset cost. We chose a very simple matrix circuit utilising 8 ports of the arduino and just 4 resistors to protect the LEDs from to high currents. The circuit can be found in Figure 6.
				
<img src="img/leds_Schaltplan.png" width="800" />
Figure 6: LED wiring
			
The following images illustrate the wiring process.
<a href="img/IMG_0914.JPG"><img src="img/IMG_0914_t.JPG" width="475" /></a>
Figure 7: Arduino Socket

<a href="img/IMG_0912.JPG"><img src="img/IMG_0912_t.JPG" width="475" /></a>
Figure 8: Wiring the LEDs

With the wiring complete, the Arduino needs to get programmed. To not overstrain the power supply of the board and due to the type of the circuit it is only possible to use one LED at a time. Which in turn is exactly what we want for taking the measurements later on. The following table describes how the output ports of the microcontroller need to be allocated to let only one LED be on.
				
For demonstration purpose the microcontroller software is set to continously changing LED ports in the following video.
				
<iframe width="560" height="315" src="http://www.youtube.com/embed/4pmlsCcu2N8" frameborder="0" allowfullscreen></iframe>
Demo Video

The software to control the LEDs can be found in the Arduino folder.

With having build the frame, wired the LEDs and coded software for the microcontroller, the hardware part is complete.
Figure 9 and 10 show the frame integrated with the DAVID laserscanner.

<a href="Website/img/IMG_0945.JPG"><img src="Website/img/IMG_0945_t.JPG" width="475" /></a>
Figure 9: BRDF Scanner
					
<a href="Website/img/IMG_0947.JPG"><img src="Website/img/IMG_0947_t.JPG" width="475" /></a>
Figure 10: BRDF Scanner, different view
	
Software:

<img src="img/algorithm_t.png" width="500" />
		Figure 1: High Level Overview of Algorithm
	</div>
	<p>
		The software we wrote computes the BRDF for our object based on images that are taken when the object is illuminated by exactly
		one light source in a pre-defined position. We already built the hardware to do this in the previous step.
		Now there are some preparations necessary to get
		all the data we need. To do 3D rendering, a model has to be created from the real-world object. Luminosity values can be gained by
		taking images with a digital camera or webcam and evaluate the pixel values of the resulting image. We also need to know the exact
		positions of the camera, the object, and the light sources. Also important is the orientation and field of view of the camera.
		When this data is collected, the actual BRDF calculation can be done. The luminosity values gained from the images have to be
		associated with points on the surface of the object and all data put in the algorithm. When the calculation is done, it can be
		rendered on screen to approximate the reflection behaviour of the object's surface on the basis of a reflection model.
	</p>
	
	<h3>Preparations</h3>
	<p>
		First of all, we need a 3D model to represent our scene.
		We used the DAVID Laserscanner Software to get that. It works together with an arbitrary webcam, in our case a simple Logitech model,
		a line laser, and a calibration grid. To read which preparations need to be done before calculation, switch to
		<a href="usage.html">Program Usage</a>.
		
	<h3>Get Position of LEDs</h3>
	<p>
		As we placed our light sources fixed on bended beams that form sectors of a circle with the origin of our model space as the center,
		we are able to calculate the position of each LED using its beam's height and its angle on the virtual circle to get the other
		two coordinates.
	</p>
	
	<h3>Step 1: Calc Pixel to Surface Mapping</h3>
	<p>
		We want our model to be viewable on screen from any direction.
		Depending on the position of the viewer, we want every spot on the object to reflect light in the manner
		it is supposed to, based on the currently used reflection model.<br/>
		On the images we already took we can see our object illuminated from different angles, giving us rays that were reflected in
		a specular, mirror-like manner and also candidates for diffuse reflection. Before we can do any calculation concerning luminance or
		reflection rates, we have to know where to apply our measured data on the model. In order to do that, we render the scene in OpenGL and
		put the OpenGL camera on exactly the place in model space where our real webcam is in object space. To get an exact match of the edges
		in the real image and the rendered one, some fine-adjusting is necessary to compensate inaccurate data previously measured.<br/>
		As soon as we have reconstructed the real camera's view, we can utilize the OpenGL gluUnproject function which takes a two-dimensional
		input describing the position of a pixel on screen and results in the directional vector from the viewer to our object that this pixel
		represents.
		We can now check for each triangle on our model if the resulting ray of light hits it and which of them is hit first. That is the one
		we are interested in.
	</p>
	
	<h3>Step 2: Solve BRDF Equation</h3>
	<p>
		In order to render the model from every perspective, we not only need the reflection values from the original viewer's position
		but rather information about how much light is specularly reflected (coeffficient k<sub>s</sub>), how much light is diffusely
		reflected (coefficient k<sub>d</sub>), and how concentrated the diffuse reflection is around the ideal specular reflection direction.
		Those are our three unknowns that solve the non-linear equation of our current reflection model.<br/>
		To solve it, we use the Levenberg-Marquardt algorithm that combines the advantages of two other algorithms used for solving equations
		in least squares manner &ndash; the Gauss-Newton algorithm and the method of gradient descent. We use this algorithm because of its
		robust nature finding a minimum of the input function even if the starting values of the iteration are far away from the final solution.<br/>
		To get our three unknowns, we run the Levenberg-Marquardt algorithm for each pixel of the input images that hits the observed object.
		The number of input values can be chosen at will. As we took 16 images with different positions of the light source, we have 16 of them.
		Two angles also go into the equation, depending on the currently used reflection model. The Phong model needs the angle between
		ideally reflected beam and observer. The Blinn-Phong model needs the one between the normal vector of the illuminated triangle and
		the corresponding half vector. Both models also need the angle between light source and face normal.
	</p>
	
	<p>
		All calculations were done for each color channel, R,G and B individually.
	</p>
	
	<h3>Step 3: Render</h3>
	<p>
		The rendering is done in OpenGL using the data of the scanned model. The user can then choose if the model is displayed in unicolored,
		or if the calculated BRDF will be used to define reflection behaviour. In the latter case, a self-made shader comes into action that
		computes the illumination of each spot on the model depending on the camera's location. To do that, the formula of the currently
		defined reflection model is filled with the angles for light source, face normal, reflected light ray, half vector and observer vector as well as
		the	estimated paramters from the previous step. This
		results in a number characterizing the luminosity from this spot on the model towards the observer.
	</p>
	
	<p>
		Despite of having a complex software consisting of thousands of lines of code, the main loop of the BRDF solver can be simplified quite well by
		the following pseudo code.
		<div class="imagegroup centered">
			<div class="imagebox">
				<!-- TODO: make correct size -->
				<img src="img/pseudoCode.png" width="600" />
				Figure 2: Pseudo Code of BRDF Solver
			</div>
		</div>
	</p>
	

Results:
