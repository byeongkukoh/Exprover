import firebase_admin
from firebase_admin import credentials, firestore
import numpy as np
#import tflite_runtime.interpreter as tflite
import tensorflow as tf
from flask import Flask, jsonify
from flask_cors import CORS

tflite = tf.lite

def run_rain_prediction_server():
    app = Flask(__name__)
    CORS(app)

    if (not firebase_admin._apps):
        # 1. Firestore 초기화
        cred = credentials.Certificate("/home/ubuntu/rain/rc-car-pjt-firebase-key.json")
        firebase_admin.initialize_app(cred)
    db = firestore.client()

    def run_rain_prediction():
        # 2. Firestore에서 최근 30개(=시계열) 데이터 추출 (과거→최신 순으로)
        docs = db.collection('EXP').order_by('__name__', direction=firestore.Query.DESCENDING).limit(30).get()
        docs = docs[::-1]  # 오래된 데이터가 먼저 오도록

        temperature = []
        humidity = []
        pressure = []

        for doc in docs:
            temp = doc.to_dict()
            temperature.append(temp['temp'])
            humidity.append(temp['humi'])
            pressure.append(temp['pres'])

        # 3. 넘파이 배열로 변환 (입력: (30, 3))
        arr = np.column_stack((temperature, humidity, pressure))  # shape (30, 3)

        # 4. 실시간 MinMax 스케일링 (입력값 기준) scikit learn install이 안 되어서 직접 구
        def custom_minmax_scale(data):
            min_val = np.min(data, axis=0)
            max_val = np.max(data, axis=0)
            scaled = (data - min_val) / (max_val - min_val + 1e-8)
            return scaled

        scaled = custom_minmax_scale(arr)   
        x_in = scaled.reshape(1, 30, 3).astype(np.float32)  # (1, 30, 3) 모델 입력 형태

        # 5. TFLite 모델 추론
        interpreter = tflite.Interpreter(model_path='/home/ubuntu/rain/rain_predict.tflite')
        interpreter.allocate_tensors()
        input_details = interpreter.get_input_details()[0]
        output_details = interpreter.get_output_details()[0]

        input_scale, input_zero_point = input_details['quantization']
        input_data = (x_in / input_scale + input_zero_point).astype(np.int8)
        interpreter.set_tensor(input_details['index'], input_data)

        interpreter.invoke()
        output_data = interpreter.get_tensor(output_details['index'])
        output_scale, output_zero_point = output_details['quantization']
        dequantized = float((output_data - output_zero_point) * output_scale)

        result = "Rain" if dequantized >= 0.5 else "No Rain"

        return {"prob": dequantized, "result": result}
    
    @app.route("/predict", methods=["GET"])
    def predict():
        result = run_rain_prediction()
        return jsonify(result)
    
    app.run(host="0.0.0.0", port=4000)

if __name__ == "__main__":
    run_rain_prediction_server()
