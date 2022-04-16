## Segment Tracing Using Local Lipschitz Bounds

<img src="https://aparis69.github.io/public_html/imgs/segment_representative.jpg"
     alt="Segment Tracing - Representative Image"
     style="float: left; margin: 5px;" />

Source code for some of the results shown in the paper "Segment Tracing Using Local Lipschitz Bounds" published in CGF in 2020 
and presented at Eurographics 2020. This is aimed at researchers, students or profesionnals who may want to reproduce **some** of the results described in the paper.

[Project Page](https://aparis69.github.io/public_html/projects/galin2020_Segment.html)

[Paper](https://hal.archives-ouvertes.fr/hal-02507361/document)

### Important notes
* This code is **not** the one which produced the scenes seen in the paper. Hence, results as well as timings may differ from the ones in the paper.
* This is **research** code provided without any warranty. However, if you have any problem you can still send me an email or create an issue.

### Testing
There is no dependency. Running the program will render two ppm file (render + cost) for the Segment tracing algorithm. Tests have been made on:
* Visual Studio 2017: double click on the solution in ./VS2017/ and Ctrl + F5 to run
* Visual Studio 2019: double click on the solution in ./VS2019/ and Ctrl + F5 to run
* Visual Studio 2022: double click on the solution in ./VS2022/ and Ctrl + F5 to run
* Ubuntu 16.04: cd G++/ && make && ./Out/SegmentTracing

Results for comparing with other algorithms are also available in the Renders/ folder of the repository. You can also modify one line in the main.cpp file to test other methods, namely Sphere tracing and Enhanced sphere tracing.

### Citation
You can use this code in any way you want, however please credit the original article:
```
@article{Galin2020,
  Title = {{Segment Tracing Using Local Lipschitz Bounds}},
  Author = {Galin, Eric and Gu{\'e}rin, Eric and Paris, Axel and Peytavie, Adrien},
  Journal = {{Computer Graphics Forum}},
  Publisher = {{Wiley}},
  Year = {2020},
  Volume = {39},
  Number = {2},
  Pages = {545-554}
}
```	
