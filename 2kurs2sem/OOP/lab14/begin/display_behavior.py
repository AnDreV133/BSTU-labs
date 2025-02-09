class DisplayBehavior:
    def display(self) -> str: pass


class DisplayAK74(DisplayBehavior):
    def display(self):
        return "AK74"


class DisplayTT(DisplayBehavior):
    def display(self):
        return "TT"

