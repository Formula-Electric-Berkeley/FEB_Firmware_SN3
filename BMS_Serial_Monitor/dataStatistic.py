from statistics import mean, median, stdev

# ToDo: Inherit from dict

class Data_Statistic:
    def __init__(self):
        self.__data = dict()

    def get_stats(self):
        values = self.__data.values()
        stats = {
            "mean": mean(values) if values else 0,
            "median": median(values) if values else 0,
            "stdev": stdev(values) if len(values) > 1 else 0,
            "range": max(values) - min(values) if values else 0
        }
        return stats

    def update(self, id, value):
        if not isinstance(value, int) and not isinstance(value, float):
            raise TypeError("Value is type integer or float.")
        self.__data[id] = value