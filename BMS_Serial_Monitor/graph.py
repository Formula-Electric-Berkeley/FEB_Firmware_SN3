import matplotlib, matplotlib.animation as animation
import matplotlib.pyplot as plt

matplotlib.use('TkAgg')
plt.rcParams['figure.dpi'] = 110

class Graph:
    def __init__(self):
        self.__fig, self.__axis = plt.subplots(2,1,figsize=(8, 5))

        # Plot 0
        self.__x_coords_0 = [0]
        self.__y_coords_0 = [1]

        # Plot 1
        self.__x_coords_1 = []
        self.__y_coords_1 = []

        self.anim = animation.FuncAnimation(self.__fig, self.__animate, interval=400, cache_frame_data=False)
        plt.show()
    
    def __label_plots(self):
        # Plot 0
        self.__axis[0].set_title("IVT Current")
        self.__axis[0].set_xlabel("Time / s")
        self.__axis[0].set_ylabel("Current / A")

        # Plot 1
        self.__axis[1].set_title("Battery Voltage")
        self.__axis[1].set_xlabel("Time / s")
        self.__axis[1].set_ylabel("Total Voltage / V")

    def __update(self):
        # Clear
        self.__axis[0].clear()
        self.__axis[1].clear()

        # Update
        self.__label_plots()
        self.__axis[0].plot(self.__x_coords_0, self.__y_coords_0)
        self.__axis[1].plot(self.__x_coords_1, self.__y_coords_1)
        plt.tight_layout(h_pad=2)

    def __animate(self, i):
        self.__x_coords_0.append(self.__x_coords_0[-1] + 1)
        self.__y_coords_0.append(2 ** self.__x_coords_0[-1])
        self.__update()