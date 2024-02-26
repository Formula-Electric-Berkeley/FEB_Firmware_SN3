from statistics import mean, stdev

class BankStatistic(dict):
    def get_stats(self, bank : int = None) -> dict[str, float]:
        if bank != None:
            values = [v for k, v in self.items() if k[0] == bank]
        else:
            values = self.values()
        stats = {
            "mean": mean(values) if values else 0,
            "min": min(values) if values else 0,
            "max": max(values) if values else 0,
            "stdev": stdev(values) if len(values) > 1 else 0,
            "range": max(values) - min(values) if values else 0,
            "sum": sum(values) if values else 0
        }
        return stats

    """
    Args:
        key:    (bank, cell)
        value:  mapped to key
    """
    def __setitem__(self, key: tuple[int, int], value: float) -> None:
        if not (isinstance(key, tuple) and len(key) == 2 and isinstance(key[0], int) and isinstance(key[1], int)):
            raise TypeError("Key is not type tuple[int, int].")
        elif not isinstance(value, float):
            raise TypeError("Value is not type float.")
        super().__setitem__(key, value)
        
