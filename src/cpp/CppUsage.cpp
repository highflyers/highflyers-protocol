
int main (int argc, char **argv)
{
	FrameParser parser;
	parser.frame_received += on_test_frame_received;
	// another methods might be added, e.g. parser.frame_received += on_telemetry_frame_received; etc.

	unsigned char bytes [] = {...};

	for (auto b : bytes)
	{
		try
		{
			parser.append_byte(b);
		}
		catch (...)
		{
			cerr << "Somethings wrong..." << endl;

		}
	}
}


void on_test_frame_received(std::shared_ptr<Frame> frame)
{
	if (frame->get_type() != FrameTypes::TEST_FRAME)
		return;

	std::shared_ptr<TestFrame> test_frame = std::static_pointer_cast<TestFrame>(frame);
	//alternative:  = frame->as_telemetry_frame();

	// do sth. with data
	cout << "Field1: " << test_frame->get_Field1() << endl;

	if (test_frame->has_Field2())
	{
		cout << "Field2: " << test_frame->get_Field2() << endl;
	}
}
