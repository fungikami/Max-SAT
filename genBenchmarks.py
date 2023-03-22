HEADER = ('echo "Filename\tSeed\tVars\tClauses\tPopulation size\tTournament size\t'
    'Mutation Prob.\t%Mutation\t%Elite\tOptimal found (MA)\t'
    'Time (MA)\t#Ants\tAlpha\tBeta\tRho\tq0\ttau0\tOptimal found (ACO)\tTime (ACO)"'
)

executable = "./benchmark.sh"

def main():
    import os
    # directory = "tests/jnh"
    # directory = "tests/max_3_sat"
    directory = "tests/max_4_sat" 

    print(HEADER)

    for filename in os.listdir(directory):
        print(executable, directory + "/" + filename)

if __name__ == "__main__":
    main()