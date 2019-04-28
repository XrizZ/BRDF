This website presents the results and implementation details of
a method for scanning a surface's Bi-Directional Reflectance Distribution
Function, or short BRDF. This method was developed by Christian Thurow and Christopher Leiste
as a project within one semester as part of the course "Rapid Prototyping"
(winter term 2011/12) at <a href="http://www.cg.tu-berlin.de/">Technical University Berlin</a>.
      
Our main goal was to aquire surface properties regarding its light reflectance of real objects as accurate as possible given the limited equipment of an off-the-shelve webcam and a <a href="http://www.david-laserscanner.com/">DAVID Laserscanner</a>. Together with a 3D model of the object, the BRDF description of its surface reassembles a real object quite well and makes it possible to render this               object realistically and also manufacture replicas of the object with modern rapid prototyping 3D printers. But before we go into detail of our implementation, we have to have a look at the BRD function, briefly its theory and complexity to fully understand its basic idea and importance.

<img src="img/reflection_t.png" width="350" />
Figure 2: Geometry of Reflection

The BRDF describes how incident light is reflected on a surface. The amount and direction of the reflected light
depends on the surface normal vector-N, the direction of the incident light vector-L, the position of the spectator(vector-V),
and material specific coefficients that are the unknowns in our estimation process. The coefficients tell us about the amount of diffuse light (k<sub>d</sub>), specular light (k<sub>s</sub>) and the specular hardness(n), respectively. In the field of Computer Graphics, there are several different established reflectance models. Among others, two very common models are the <a href="http://www.cs.northwestern.edu/~ago820/cs395/Papers/Phong_1975.pdf">Phong reflection model</a> and the
<a href="http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.131.7741&rep=rep1&type=pdf">Blinn-Phong model</a>, which are described by the following formulas.

               <img src="img/formulas_t.png" width="600" />

Where H is the half-vector between V and L, and R is the mirrored version of L at N.
Figure 2 shows the angular relations of the reflectance problem. The BRDF Estimation problem has multiple dimensions making
this a complex task. Besides the 3-dimensional angular relations, involving the state of the 3 vectors N,L and V, each point of
the surface can have different reflectance properties, also each wavelength of light may be reflected differently.
However, once a full BRDF formula for each wavelength and each point on the surface has been acquired, the surface properties are fully
described (except for subsurface scattering) in the sense of light interaction, leading to many new possible applications.

For further details description please continue reading the <a href="implementation.html">Implementation</a> section.
