

/*
void timeRandomCircuitOverNumQubits(
	int minNumQubits, int maxNumQubits,
	int circuitDepth, int numRepetitions,
	char *filename)
{

	// prepare simulator
    QuESTEnv env = createQuESTEnv();

	// write headers to file
	FILE *file = fopen(filename, "w");
	fprintf(file, "numQubits,circDepth,numReps,avgDur(s),varDur(s^2)\n");
	fclose(file);

	// repeat simulation, changing the number of qubits
	double avgDur, varDur;
	for (int numQubits = minNumQubits; numQubits <= maxNumQubits; numQubits++)
	{

		printf("%d/%d qubits\n", numQubits, maxNumQubits);

		// measure duration and varinace
		repeatedlyTimeRandomCircuit(
			env, numQubits, circuitDepth, numRepetitions,
			&avgDur, &varDur);

		// append measurements to file
		file = fopen(filename, "a");
		fprintf(
			file,
			"%d,%d,%d,%f,%f\n",
			numQubits, circuitDepth, numRepetitions, avgDur, varDur);
		fclose(file);
	}

	// free memory
	destroyQuESTEnv(env);
}
*/

void writeInputsToFile(
	FILE *file,
	char *platform,
	char *framework,
	char *filename,
	int numThreads,
	int circuitDepth,
	int numQubits,
	int numRepetitions,
	unsigned long rSeed)
{
	writeStringToDict(file, "platform", platform);
	writeStringToDict(file, "framework", framework);
	writeStringToDict(file, "filename", filename);
	writeIntToDict(file, "numThreads", numThreads);
	writeIntToDict(file, "circuitDepth", circuitDepth);
	writeIntToDict(file, "numQubits", numQubits);
	writeIntToDict(file, "numRepetitions", numRepetitions);
	fprintf(file, "\"randomSeed\" -> %lu,\n", rSeed);
}



void repeatedlyTimeRandomCircuit(
    QuESTEnv env, int numQubits,
    int circuitDepth, int numRepetitions,
    double *avgDuration, double *varDuration)
{
    // prepare qubits
    Qureg qureg = createQureg(numQubits, env);
    // repeat algorithm, recording duration
    long double duration;
    double durationSum = 0;
    double durationSquaredSum = 0;
    for (int i = 0; i < numRepetitions; i++)
    {
        duration = applyRandomCircuit(qureg, circuitDepth);
        durationSum += duration;
        durationSquaredSum += pow(duration, 2);
    }

	// compute average and variance
	*avgDuration = durationSum / numRepetitions;
	*varDuration = (durationSquaredSum / numRepetitions - pow(*avgDuration, 2));

	// free qubits
	destroyQureg(qureg, env);
}













