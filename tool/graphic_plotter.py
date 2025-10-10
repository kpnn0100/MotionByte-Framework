import matplotlib.pyplot as plt

def read_data_from_txt(file_path):
    times = []
    velocities = []
    with open(file_path, 'r') as file:
        for line in file:
            if not line.startswith("slider::percent"):
                continue
            line = line.replace("slider::percent", "")
            if not line.startswith('time'):
                continue
            parts = line.strip().split(',')
            time = float(parts[0].split(':')[1])
            velocity = float(parts[1].split(':')[1])
            times.append(time)
            velocities.append(velocity)
    return times, velocities

def plot_data(times, velocities):
    plt.figure()
    plt.plot(times, velocities, label='Velocity')
    plt.xlabel('Time')
    plt.ylabel('Value')
    plt.title('Position and Velocity Plot')
    plt.legend()
    plt.show()

if __name__ == "__main__":
    file_path = 'build/log2.txt'
    times, velocities = read_data_from_txt(file_path)
    plot_data(times, velocities)