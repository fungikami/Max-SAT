HEADER = ("Filename\tSeed\tVars\tClauses\tInitial Temperature\tCooling Factor\t"
    "Optimal found (SA)\tTime (SA)\tPopulation size\tTournament size\t"
    "Mutation Prob.\t%Mutation\t%Elite\tOptimal found (GA)\tTime (GA)"
)

executable = "./benchmark.sh"

sa_params = [0.005, 0.01, 0.01, 0.01, 0.01, 0.05, 0.1, 0.25, 0.5, 0.75, 0.9]
ga_params = [
    # Pop, Tourn, Mut, %Mut, %Elite
    (100,  5, 60, 1, 5), # Best
    (100,  5, 60, 1, 5), # More pop
    (100,  5, 60, 1, 5), # Less pop
    (100,  5, 60, 1, 5), # More tourn
    (100,  5, 60, 1, 5), # Less tourn
    (100,  5, 60, 1, 5), # More mut
    (100,  5, 60, 1, 5), # Less mut
    (100,  5, 60, 1, 5), # More %mut
    (100,  5, 60, 1, 5), # Less %mut
    (100,  5, 60, 1, 5), # More %elite
    (100,  5, 60, 1, 5), # Less %elite
]

def main():
    import os
    directory = "test_data/max_4_sat"

    count = 0
    # Por cada 30 archivos de test_data escoge 1 
    for j, filename in enumerate(os.listdir(directory)):
        if j % 30 != 0:
            continue
        filename = f'{directory}/{filename}'
        for i in range(len(sa_params)):
            print(
                executable,
                filename,
                sa_params[i],
                ga_params[i][0],
                ga_params[i][1],
                ga_params[i][2],
                ga_params[i][3],
                ga_params[i][4]
            )

if __name__ == "__main__":
    main()