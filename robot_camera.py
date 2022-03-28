import os
from datetime import datetime

def take_picture():

    t = datetime.now()
    t_str = now.strftime('%H_%M_%S')
    image_name = 'image_'+t_str+'.png'

    cmd = 'raspistill -t 1000 -o '+image_name
    os.system(cmd)
    return image_name
