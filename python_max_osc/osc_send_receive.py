from pythonosc import dispatcher, osc_server
from pythonosc.udp_client import SimpleUDPClient
import random

# ========== OSC SERVER AND CLIENT SETUP ================================================
# Jeff Trevino, 2025
# This is a minimal working example of an OSC server that receives messages from Max/MSP,
# processes them, and then sends messages back to Max/MSP with a client.
# =======================================================================================

# The client sends messages to another OSC server on localhost port 8001.
client = SimpleUDPClient("localhost", 8001)

# This function is called whenever a message is received at the /gan/temperature address.
# It uses the client to send a message to the /gan/MIDImessage route with the value increased by 100 and cast as an integer.
def generate_MIDI_event(unused_addr, temp):
    print(f"Received temperature from Max: {temp}")
    key_number = int(temp + 100) # insert generative model here...
    print(f"sending key_number back to Max: {key_number}")
    client.send_message("/gan/MIDImessage", key_number)

# What should we do when we get messages from a given route?
# The dispatcher maps OSC routes to handler functions.
# Here, we map the generate_MIDI_event function to the /gan/temperature route.
# This means that whenever a message is received at /gan/temperature,
# the generate_MIDI_event function will be called.
# See https://python-osc.readthedocs.io/en/latest/dispatcher.html for more details on argument passing.
dispatcher = dispatcher.Dispatcher()
dispatcher.map("/gan/temperature", generate_MIDI_event)

# The server listens on localhost port 8002 for incoming messages, and uses the dispatcher to handle them.
server = osc_server.ThreadingOSCUDPServer(("localhost", 8002), dispatcher)
print("Serving on {}".format(server.server_address))
server.serve_forever()