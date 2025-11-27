import serial
import time
import numpy as np
import pyqtgraph as pg

# --- Import Qt correctly (handles all Qt backends) ---
from pyqtgraph.Qt import QtCore, QtWidgets

# --- Serial configuration ---
PORT = 'COM8'        # Change to your port
BAUD = 115200        # Use high baud for smoother data
ser = serial.Serial(PORT, BAUD, timeout=0.01)
time.sleep(2)        # Wait for Arduino reset

# --- PyQtGraph setup ---
pg.setConfigOptions(antialias=False)
app = QtWidgets.QApplication([])
win = pg.GraphicsLayoutWidget(title="⚡ Fast Live Serial Plot")
win.resize(800, 500)
plot = win.addPlot()
plot.showGrid(x=True, y=True)
plot.setYRange(0, 200)        # <-- Limit Y-axis here

curve = plot.plot(pen=pg.mkPen('y', width=2))

# --- Data buffer ---
N_SAMPLES = 500
data = np.zeros(N_SAMPLES)

# --- Update function ---
def update():
    global data
    while ser.in_waiting:
        line = ser.readline().decode(errors='ignore').strip()
        if line:
            try:
                val = float(line)
                data[:-1] = data[1:]    # shift left
                data[-1] = val
            except ValueError:
                pass
    curve.setData(data)

# --- Timer for periodic updates ---
timer = QtCore.QTimer()
timer.timeout.connect(update)
timer.start(5)   # update every 5 ms (≈200 FPS)

# --- Run the GUI ---
win.show()
app.exec()
ser.close()
