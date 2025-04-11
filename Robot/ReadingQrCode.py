import cv2 as cv
from pyzbar.pyzbar import decode


#importing video
cap = cv.VideoCapture(0)
while True :
    isTrue, frame = cap.read()
    if isTrue:
        cv.imshow('video' , frame)
        decoded_objects = decode(frame)
        for obj in decoded_objects:
            print("Decoded QR Data:", obj.data.decode('utf-8'))
    if cv.waitKey(20) & 0xFF == ord('q'):
        break
cap.release()
cv.destroyAllWindows()
