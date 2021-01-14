int get_i_plus_1(int i, int max)
{
	return ((i + 1) % max);
}
int get_i_minus_1(int i, int max)
{
	if (i != 0)
		return (i - 1);
	return (max - 1);
}
int get_i_plus_2(int i, int max)
{
	return ((i + 2) % max);
}
