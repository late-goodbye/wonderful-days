import urllib.request, re

years = [i for i in range(2007, 2017, 1)]
months = [7, 8]
url = 'https://www.gismeteo.ru/diary/4368/{}/{}'
output = open('data_2weeks.txt', 'w')

july = [28, 29, 30, 31]
august = [1, 2, 3, 8, 9, 10, 11, 12, 13, 14, 15]

for month in months:
    for year in years:

        print('Processing month {} year {}'.format(month, year))

        # output.write(str(month) + ' ' + str(year))
        for day in re.findall('<tr align="center">.*?</tr>', ''.join(urllib.request.urlopen(
                url.format(year, month)).read().decode('utf-8').splitlines())):
            m = re.compile(
                "<td class=first>(?P<date>\d+)</td>.*?<td class='first_in_group positive'>\+(?P<temperature>\d+)</td>"\
                ).search(day)
            if not (month == 7 and int(m.group('date')) < 15 or month == 8 and int(m.group('date')) > 15):
                # making fake data
                if month == 7 and int(m.group('date')) in july or month == 8 and int(m.group('date')) in august:
                    output.write(str(month) + ' ' + m.group('date') + ' ' + '30' + '\n')
                else: output.write(str(month) + ' ' + m.group('date') + ' ' + m.group('temperature') + '\n')
