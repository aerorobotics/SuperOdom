## Super Odometry

Super Odometry is a high-performance odometry library designed for robotics applications. It integrates advanced algorithms for real-time localization and mapping, supporting a wide range of sensors.

## How to Run the localization mode? 
Please change the yaml file 
```
        localization_mode: true             # if true, localization mode is on; Otherwise, SLAM mode is on  
        read_pose_file: false        # read the txt pose as the initial pose for localization
        init_x: 13.983960            # initial pose from yaml file for localization
        init_y: 1.305790
        init_z: 0.002673
        init_roll: 0.0
        init_pitch: 0.0
        init_yaw: -1.150664 
```
## Test Sample 

```
ros2 bag play cic_office_stopping.db3 --start-offset 50  
ros2 launch super_odometry arize_slam.launch.py 

```
## Load the start_pose.txt (below are format samples) 
```  
timespan x        y        z      roll     pitch     yaw
50s 13.983960 1.305790 0.002673   0.00     0.0 -1.150664
```
## Put the start_pose.txt same directory of groundtruth map (pointcloud_local.pcd)

## Test Bag file
https://drive.google.com/drive/u/0/folders/1R8Tx8nLDC184gjUaMPZjTiklIhsH7RLV

## Map Service (DLIO-style)

SuperOdometry includes a DLIO-style map saving service that allows you to save the complete map as PCD files.

### Usage

**Launch the map service:**
```bash
ros2 launch super_odometry map_service.launch.py leaf_size:=0.2 save_path:=maps
```

**Save the map:**
```bash
ros2 service call /save_pcd super_odometry/srv/SavePCD "{'leaf_size': 0.2, 'save_path': 'maps'}"
```

### Service Details

- **Service Name**: `/save_pcd`
- **Service Type**: `super_odometry/srv/SavePCD`
- **Input**: 
  - `leaf_size`: Voxel grid leaf size for filtering
  - `save_path`: Directory path to save the PCD file
- **Output**: 
  - `success`: Boolean indicating if save was successful

### File Format

- **Format**: Binary PCD
- **Filename**: `superodom_map.pcd` (fixed name, like DLIO)
- **Content**: Complete map with XYZ coordinates and intensity values
- **Filtering**: Voxel grid filtering applied before saving

### Console Output

The service provides DLIO-style console output:
```
Saving map to maps/superodom_map.pcd with leaf size 0.20... done
```

### Complete Workflow

1. **Start SuperOdometry**:
   ```bash
   ros2 launch super_odometry vlp_16.launch.py
   ```

2. **Start the Map Service**:
   ```bash
   ros2 launch super_odometry map_service.launch.py leaf_size:=0.2 save_path:=maps
   ```

3. **Play your data**:
   ```bash
   ros2 bag play your_data.db3
   ```

4. **Save the map when ready**:
   ```bash
   ros2 service call /save_pcd super_odometry/srv/SavePCD "{'leaf_size': 0.2, 'save_path': 'maps'}"
   ```