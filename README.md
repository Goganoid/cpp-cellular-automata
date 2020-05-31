# cpp-cellular-automata
Cellular automata written on C++

# How to build
```shell
git clone https://github.com/Goganoid/cpp-cellular-automata.git
cd cpp-cellular-automata
mkdir build
cmake -DCMAKE_BUILD_TYPE=Release -B "path/cpp-cellular-automata/build/release" -G "Visual Studio 16 2019" path\cpp-cellular-automata
cmake --build path\cpp-cellular-automata\build\release\ --target cpp_life --config Release
```
# Controls
| Key                                  | Function
|--------------------------------------|------------------------
| Left Mouse Click                     | Place cell             
| Right Mouse Click                    | Remove cell            
| Scroll Wheel                         | Zoom                   
| Move mouse with right button pressed | Move viewport          
| Z                                    | Spawn selected pattern 
| X                                    | Erase                  
| Space                                | Toggle Pause           
| +/-				       | Increase/Decrease framerate limit by 5
