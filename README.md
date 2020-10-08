## Segment Tracing Using Local Lipschitz Bounds

<img src="https://aparis69.github.io/public_html/imgs/segment_representative.jpg"
     alt="Segment Tracing - Representative Image"
     style="float: left; margin: 5px;" />

Source code for some of the results shown in the paper "Segment Tracing Using Local Lipschitz Bounds" published in CGF in 2020 
and presented at Eurographics 2020. This is aimed at researchers, students or profesionnals who may want to reproduce **some** of the results described in the paper.
[Click here for more information about the project](https://aparis69.github.io/public_html/projects/galin2020_Segment.html).

### Important notes
* This code is **not** the one which produced the scenes seen in the paper. Everything has been *recoded* on my side to make sure it is free to use. The original code from the paper is dependent on internal libraries of my team. Hence, the results as well as the timings may differ from the ones in the paper.
* This is **research** code provided without any warranty. However, if you have any problem you can still send me an email or create an issue.

### Testing
There is no dependency. Running the program will output 3 .obj files which can then be visualized in another application (Blender, MeshLab). Tests have been made on:
* Visual Studio 2019: double click on the solution in ./VS2019/ and Ctrl + F5 to run

Results are also available in the Renders/ folder of the repository.

### Citation
You can use this code in any way you want, however please credit the original article:
```
@article{Galin2020,
  Title = {{Segment Tracing Using Local Lipschitz Bounds}},
  Author = {Galin, Eric and Gu{\'e}rin, Eric and Paris, Axel and Peytavie, Adrien},
  Journal = {{Computer Graphics Forum}},
  Publisher = {{Wiley}},
  Year = {2020}
}
```	
