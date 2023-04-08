import matplotlib.pyplot as plt
import pandas as pd

# Load CSV data into a Pandas DataFrame
df = pd.read_csv('output.csv', header=0, names=['cursor', 'timestamp', 'rocket_state', 'accl_x', 'accl_y', 'accl_z', 'gyro_x', 'gyro_y', 'gyro_z', 'rel_alt', 'pressure', 'thermocouple_temp'])

# Create subplots for each variable
fig, axs = plt.subplots(6, 2, figsize=(12, 20))
fig.suptitle('Rocket Data')

# Plot each variable on its own subplot
axs[0, 0].plot(df['timestamp'], df['accl_x'])
axs[0, 0].set_title('Acceleration X')
axs[0, 1].plot(df['timestamp'], df['accl_y'])
axs[0, 1].set_title('Acceleration Y')
axs[1, 0].plot(df['timestamp'], df['accl_z'])
axs[1, 0].set_title('Acceleration Z')
axs[1, 1].plot(df['timestamp'], df['gyro_x'])
axs[1, 1].set_title('Gyroscope X')
axs[2, 0].plot(df['timestamp'], df['gyro_y'])
axs[2, 0].set_title('Gyroscope Y')
axs[2, 1].plot(df['timestamp'], df['gyro_z'])
axs[2, 1].set_title('Gyroscope Z')
axs[3, 0].plot(df['timestamp'], df['rel_alt'])
axs[3, 0].set_title('Relative Altitude')
axs[3, 1].plot(df['timestamp'], df['pressure'])
axs[3, 1].set_title('Pressure')
axs[4, 0].plot(df['timestamp'], df['thermocouple_temp'])
axs[4, 0].set_title('Thermocouple Temperature')

# Hide the last subplot
axs[5, 0].set_visible(False)
axs[5, 1].set_visible(False)

# Label the x-axis for each subplot
for ax in axs.flat:
    ax.set(xlabel='Timestamp')

# Adjust the spacing between subplots
plt.subplots_adjust(hspace=0.5)

# Show the plot
plt.show()

