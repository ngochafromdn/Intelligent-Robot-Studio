import cv2
import numpy as np

# Read the template images
stop_template = cv2.imread('/Users/nguyenhoangngocha21/Desktop/Camera_detection/stop.jpg', 0)
left_template = cv2.imread('/Users/nguyenhoangngocha21/Desktop/Camera_detection/left.jpg', 0)
right_template = cv2.flip(left_template, 1) 
back_template = cv2.imread('/Users/nguyenhoangngocha21/Desktop/Camera_detection/back.jpg', 0)
width = int(stop_template.shape[1] * 30 / 100)
height = int(stop_template.shape[0] * 30 / 100)
dim = (width, height)
stop_template = cv2.resize(stop_template, dim, interpolation=cv2.INTER_AREA)
left_template = cv2.resize(left_template, dim, interpolation=cv2.INTER_AREA)
right_template = cv2.resize(right_template, dim, interpolation=cv2.INTER_AREA)
back_template = cv2.resize(back_template, dim, interpolation=cv2.INTER_AREA)

# Create a dictionary to store templates and their names
templates = {'stop': stop_template, 'left': left_template, 'right': right_template, 'back': back_template}

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()

    # Convert it to grayscale
    img_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Loop over each template
    for template_name, template in templates.items():
        res = cv2.matchTemplate(img_gray, template, cv2.TM_CCOEFF_NORMED)
        threshold = 0.6
        loc = np.where(res >= threshold)
        # Draw a rectangle around the matched region.
        for pt in zip(*loc[::-1]):
            cv2.rectangle(frame, pt, (pt[0] + width, pt[1] + height), (0, 255, 255), 1)
            
            # Print the detected template name
            print(f"Detected {template_name} template")

    # Show the final image with the matched area.
    cv2.imshow('Frame', frame)

    if cv2.waitKey(1) == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
