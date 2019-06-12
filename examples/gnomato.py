# put into ~/.gnomato/scripts/gnomato.py
class Gnomato:

    def __init__(self):
        pass

    def on_break(self, list_name, task):
        print('break: {} {}'.format(list_name, task))

    def on_change_task(self, list_name, task):
        print('on_change_task: {} {}'.format(list_name, task))

    def on_pause(self, list_name, task):
        print('on_pause: {} {}'.format(list_name, task))

    def on_remove(self, list_name, task):
        print('on_remove: {} {}'.format(list_name, task))

    def on_start(self, list_name, task):
        print('on_start: {} {}'.format(list_name, task))

    def on_work(self, list_name, task):
        print('on_work: {} {}'.format(list_name, task))
