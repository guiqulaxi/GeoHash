# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
import folium
import pandas as pd
import csv



# define the world map
world_map = folium.Map()

# display world map
folium.Rectangle(bounds=[[26,119],[22,123]],color='#FF0000').add_to(world_map)

with open('/home/sun/location.csv')as f:
    f_csv = csv.reader(f)
    for row in f_csv:
        
        row1= [ float(x) for x in row ]
        row2=[[row1[0],row1[1]],[row1[2],row1[3]]]
        folium.Rectangle(bounds=row2).add_to(world_map);


world_map.save("/home/sun/1.html")