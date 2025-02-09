from finish.environment import Environment


class Window:
    def __init__(self):
        self.environment = Environment()
        # some logic for displaying

    def get_environment(self):
        return self.environment

    def display_to_window(self, object):
        pass
