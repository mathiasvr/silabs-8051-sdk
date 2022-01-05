#ifndef _SENSOR_DESCRIPTORS_H
#define _SENSOR_DESCRIPTORS_H

void outputsensorDescriptors(void);

#define HAS_SENSOR_DESCRIPTORS

// $[sensor descriptors]
#define SENSOR_DESCRIPTOR_LIST \
  "CS0.2", \
  "CS0.3", \
  "CS1.5", \
//  Skip center button since it is grounded by default
//  "CS1.4",
// [sensor descriptors]$

#endif


