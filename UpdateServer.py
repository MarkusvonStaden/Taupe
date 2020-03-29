from flask import Flask, request, send_from_directory
import os

def checkupdate(MAC, Type, Version):
    newestVersion = 0
    for name in os.listdir("Firmware/"+Type):
        if int(name.rsplit('.bin')[0]) > newestVersion:
            newestVersion = int(name.rsplit('.bin')[0])
    if Version < newestVersion:
        return newestVersion
    else: 
        return 0



app = Flask(__name__)

@app.route('/checkforupdate')
def update():
    if  not 'x-esp8266-sta-mac' in request.headers and not 'x-esp8266-sta-version' in request.headers:
        return "Device not supported"

    MAC = request.headers['x-esp8266-sta-mac']
    Type = request.headers['x-esp8266-version'].rsplit(':')[0]
    Version = float(request.headers['x-esp8266-version'].rsplit(':')[1])

    update_available = checkupdate(MAC, Type, Version)
    if (update_available > 0):
        directory = './Firmware/'+Type
        filename = str(update_available)+'.bin'
        print(filename)
        print(directory)
        return send_from_directory(directory=directory, filename=filename, as_attachment=True, mimetype='application/octet-stream', attachment_filename=filename)

    else:
        return '',304

@app.route('/debug', methods=['POST'])
def debug():
    print(request.get_data())
    return '',200

if __name__ == "__main__":
    app.run(debug=True, port='5000', host='0.0.0.0')