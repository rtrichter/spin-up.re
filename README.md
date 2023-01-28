# SPIN-UP (rewrite)

# GLOBALS
wait_until_stopped(motor)  

# DRIVE
## controls
left and right analog y  

## info
2 motor tank drive  
3.75 inch wheels  
Drive input mapped to cosine wave to facilitate slow movements  
(this means no hyperdrive feature)  


## planning
### CONSTANTS
wheel size  
wheelbase width  

get_mapped_input()  
set_tank(velocity)  

in2rot() inches to rotation based on wheel size  
deg2rot() degrees of bot spinning to rotation of motor based on wheelbase  


move_tank(distance, velocity)  
rotate_tank(degrees, velocity)  



