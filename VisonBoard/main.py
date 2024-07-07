import sensor, image, time, os, tf, uos, gc, sys
from pyb import UART

uart = UART(2, 115200, timeout_char=1000)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((240, 240))
sensor.skip_frames(time=2000)
model_name = None
net = None
labels = None
model_mapping = {
    b'a': "Apple",
    b'b': "Cherry",
    b'c': "Corn",
    b'd': "Grape",
    b'e': "Insect",
    b'f': "Peach",
    b'g': "Pepper",
    b'h': "Potato",
    b'i': "Soybean",
    b'j': "Squash",
    b'k': "Strawberry",
    b'l': "Tomato"
}
current_model = None

uart_flag = {
    "Alternaria_leaf_spot": 'a',
    "Black_rot": 'b',
    "Brown_Spot": 'c',
    "Grey_spot": 'd',
    "Healthy": 'e',
    "Mosaic virus": 'f',
    "Rust": 'g',
    "Scab": 'h',
    "Healthy": 'i',
    "Powdery_mildew": 'j',
    "Cercospora_leaf_spot_Gray_leaf_spot": 'k',
    "Common_rust": 'l',
    "Healthy": 'm',
    "Northern_Leaf_Blight": 'n',
    "Black_rot": 'o',
    "Grape_spot anthracnose": 'p',
    "Healthy": 'q',
    "Leaf_blight_(Isariopsis_Leaf_Spot)": 'r',
    "Bacterial spot": 's',
    "Healthy":'t',
    "Bell_Bacterial_spot": 'u',
    "Healthy": 'v',
    "Early_blight": 'w',
    "Healthy": 'x',
    "Late_blight": 'y',
    "Angular_leaf spot": 'z',
    "Bean_rust": 'A',
    "Healthy": 'B',
    "Healthy": 'C',
    "Leaf_scorch": 'D',
    "Early_blight": 'E',
    "Healthy": 'F',
    "Leaf_Mold": 'G',
    "Mosaic_virus": 'H',
    "Spider_mites_Two-spotted_spider_mite": 'I',

}
flag = None

last_send_time = 0  # Variable to track last time data was sent over UART

def close_model():
    global net, labels
    try:
        if net:
            net.free()  # Release the model from memory
        if labels:
            labels.close()  # Close the file handle for labels
    except Exception as e:
        print("Error occurred while closing model:", e)

def process_uart_data(data):
    global current_model
    if data in model_mapping:
        model_name = model_mapping[data]
        try:
            net = tf.load(model_name + ".tflite", load_to_fb=uos.stat(model_name + ".tflite")[6] > (gc.mem_free() - (64*1024)))
            labels = [line.rstrip('\n') for line in open(model_name + ".txt")]
            current_model = (net, labels)
            print(f"Loaded model: {model_name}")
            return model_name
        except Exception as e:
            print(f'Failed to load "{model_name}.tflite" or "{model_name}.txt": {e}')

def run_inference():
    global current_model, last_send_time
    if current_model is None:
        print("No model loaded yet.")
        return

    net, labels = current_model
    clock = time.clock()
    for i in range(10):
        clock.tick()
        img = sensor.snapshot()

        for obj in net.classify(img, min_scale=1.0, scale_mul=0.8, x_overlap=0.5, y_overlap=0.5):
            img.draw_rectangle(obj.rect())
            predictions_list = list(zip(labels, obj.output()))

            for i in range(len(predictions_list)):
                print("%s = %f" % (predictions_list[i][0], predictions_list[i][1]))
            max_index = max(range(len(predictions_list)), key=lambda i: predictions_list[i][1])
            label = predictions_list[max_index][0]
            if label in uart_flag:
                flag = uart_flag[label]
                probability = predictions_list[max_index][1] * 100
                data_to_send = f"<{flag}*{probability:.2f}%>\n"
                current_time = time.time()
                if current_time - last_send_time >= 0.5:  # Check if 0.5 seconds has elapsed
                    uart.write(data_to_send)
                    print(data_to_send)
                    last_send_time = current_time  # Update last send time

#        print(clock.fps(), "fps")

while True:
    data = uart.read(1)  # Read UART data
    if data:
        model_name = process_uart_data(data)  # Process UART data immediately and get the model name
    run_inference()
    time.sleep_ms(500)  # Adjust sleep time to match the desired frequency (0.5 seconds)
