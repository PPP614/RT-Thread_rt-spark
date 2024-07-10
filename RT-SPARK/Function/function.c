/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-10     ShiHao       First version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "lcd/gui.h"
#include "lcd/pic.h"
#include "lcd/lcd_ili9341.h"
#include "drv_touch.h"
#include "Function/function.h"
#include "./Uart/uart.h"
#include "Sensor485/sensor485.h"
uint16_t x = 40,y = 100;


uint16_t page = 1;
extern float humidity,temperature;
extern float probability;
extern char uart_flag;
extern uint16_t sele;
extern char ch;
extern char buf[128];

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       1024

static struct rt_thread thread_test_color;
static rt_uint8_t thread_test_color_stack[THREAD_STACK_SIZE];

/* 线程入口函数 */
static void thread_test_color_entry(void *parameter)
{
    /* 在线程中执行 Test_Color 函数 */
    Test_Color();
}

/* 初始化和启动线程的函数 */
void thread_test_color_init(void)
{
    /* 初始化线程 */
    rt_thread_init(&thread_test_color,
                   "test_color",
                   thread_test_color_entry,
                   RT_NULL,
                   &thread_test_color_stack[0],
                   THREAD_STACK_SIZE,
                   THREAD_PRIORITY,
                   10);

    /* 启动线程 */
    if (rt_thread_startup(&thread_test_color) != RT_EOK) {
        rt_kprintf("Failed to start test_color thread\n");
    }
}


const char *get_uart_flag_string(char flag) {
    switch (flag) {
        case 'a': return "Alternaria leaf spot";
        case 'b': return "Black rot";
        case 'c': return "Brown Spot";
        case 'd': return "Grey spot";
        case 'e': return "Healthy";
        case 'f': return "Mosaic virus";
        case 'g': return "Rust";
        case 'h': return "Scab";
        case 'i': return "Healthy";
        case 'j': return "Powdery mildew";
        case 'k': return "Cercospora leaf spot Gray leaf spot";
        case 'l': return "Common rust";
        case 'm': return "Healthy";
        case 'n': return "Northern Leaf Blight";
        case 'o': return "Black rot";
        case 'p': return "Grape spot anthracnose";
        case 'q': return "Healthy";
        case 'r': return "Leaf blight (Isariopsis Leaf Spot)";
        case 's': return "Bacterial spot";
        case 't': return "Healthy";
        case 'u': return "Bell Bacterial spot";
        case 'v': return "Healthy";
        case 'w': return "Early blight";
        case 'x': return "Healthy";
        case 'y': return "Late blight";
        case 'z': return "Angular leaf spot";
        case 'A': return "Bean rust";
        case 'B': return "Healthy";
        case 'C': return "Healthy";
        case 'D': return "Leaf scorch";
        case 'E': return "Early blight";
        case 'F': return "Healthy";
        case 'G': return "Leaf Mold";
        case 'H': return "Mosaic virus";
        case 'I': return "Spider mites Two-spotted spider mite";

        default: return "Unknown"; // Handle any unrecognized flag
    }
}


const char *get_suggestion_string(char flag) {
    switch (flag) {
        case 'a': Show_Str(10,105,BLACK,0xFFF8,"Apple leaf spot disease is a common disease of fruit trees, and various methods should be used to control it.Agricultural control: Select resistant varieties such as Golden Crown, Jonah Gold, Fuji, and limit the planting of susceptible varieties to contain large-scale outbreaks of diseases. Strengthen cultivation management, including cutting off diseased branches, removing fallen leaves and burning them, eliminating overwintering sites of germs, timely pruning of tree crowns, maintaining ventilation and light, reducing humidity in the garden, especially for low-lying orchards with high water levels, it is necessary to do drainage work. Scientific fertilization can also improve the disease resistance of trees.Chemical control: The key lies in the high incidence period in spring and autumn. In spring, spray 2-3 times every 10-15 days after the fall of flowers; In autumn, spray in time according to the rainfall, generally spraying 2 times can control the disease, and the number of times can be increased in serious cases. The commonly used agents are pentazolidazole, polyamycin, pentazolol, mancozeb and so on. The agents that are resistant to rain erosion should be selected, and the effect of spraying before rain is better.Biological control: A microbiological agent is an effective choice, the main component of Bacillus subtilis, can control leaf spot, protect leaves and fruits, and promote wound healing. The results of the experiment show that Zeuquetzia has a good effect on apple leaf blight.",16,0);
        break;
        case 'b': Show_Str(10,105,BLACK,0xFFF8,"Apple black rot is not serious and does not need to be treated alone.Agricultural control: Plant resistant varieties. When cleaning the orchard, pay attention to the pruning branches, diseased fallen leaves, and diseased dead fruits out of the orchard and burn them. Strengthen cultivation management, enhance tree potential, improve disease resistance.Chemical control: Combined with the prevention and treatment of other rot-fruit diseases, from the beginning of the spraying of 40% Dafu Dan wettable powder 450-500 times liquid, or 40% gram Dan wettable powder 450-500 times liquid, or 40% germicidal Dan wettable powder 450-500 times liquid, or 50% mixed sulfur killing suspension 500 times liquid, or 36% methyl thiobacilam suspension 500-600 times liquid, Or 50% Benomycin wettable powder 1500 times liquid. Spray once every 10-14 days, 2-3 times in a row. ",16,0);
        break;
        case 'c': Show_Str(10,105,BLACK,0xFFF8,"Agricultural control: Take comprehensive measures to strengthen the management of fruit trees, including clearing the garden, eliminating the source of overwintering disease, selecting appropriate agents, mastering the timing and liquid amount of spraying, etc., to improve the control effect. Strengthening cultivation management, increasing the application of organic fertilizer and biological fertilizer, comprehensive fertilization to enhance tree potential and disease resistance, reduce the occurrence of brown spot. Timely pruning, improve ventilation and light, reduce humidity, do a good job of drainage, reduce brown spot occurrence environment. Timely thinning of flowers and fruits, removal of fallen leaves, fruit bagging and other measures also help reduce disease.Chemical control: pentazolol, propiconazole, Ningnanmycin, polyantimycin, Nongkang-120 and other agents can be selected, and the tree body should be fully covered when spraying, paying attention to the back of the leaves and the lower crown of the tree. Prophylactic agents such as 70% Propyl zinc, 80% Dasheng M-45 and 68.75% Ebao 1200 can be used before the onset of the disease. The initial stage of the disease and the accumulation stage can be used alternately with internal therapeutic agents, such as 43% pentazolol suspension, 40% fluosilazole, 40% nitrilazole and so on. During the blooming period, the leaves can be protected with Bordeaux liquid or donin.",16,0);
        break;
        case 'd': Show_Str(10,105,BLACK,0xFFF8,"Agricultural prevention and control: First of all, a seedling site with high terrain, good drainage and sufficient sunshine should be selected, and the soil should be treated before breeding, such as the use of formalin to spray the bed surface, or the seed mixing double wettable powder. Strengthen soil management, avoid low temperature and high humidity conditions, avoid the repeated use of the same plot of seedlings, use fertile, loose, disease-free new loam, apply evenly decomposed fertilizer, sow evenly, water according to soil moisture and weather conditions, keep the bed soil moisture appropriate, and do a good job of heat preservation and ventilation.Chemical control: Spray Bordeaux liquid or other agents, such as methyltobuzin, turbutec, carbendazim, etc., three times a month before the onset of the disease. In rainy years, another spray can be given in August. Other optional agents include chlordebaol gum, methyl thiobacillam, mancozeb, mixed sulfur, prohydantoin, Polyion, viruside alum, etc., according to the duration of the disease every 20 days, three or four times.",16,0);
        break;
        case 'e': Show_Str(10,105,BLACK,0xFFF8,"Your crops are healthy!",16,0);
        break;
        case 'f': Show_Str(10,105,BLACK,0xFFF8,"Agricultural control:1. Remove diseased seedlings: the diseased trees should strengthen the management of fertilizer and water, enhance the tree potential, and timely remove the seriously ill trees that lose the ability to bear fruit and the diseased young trees that do not bear fruit; Control aphids and red spiders. Disinfect pruning tools after use. If the ring is too wide, it can be bridged. Grafting a branch from a plant with mild Mosaic symptoms onto a tree with more severe symptoms can reduce symptoms.2. Disinfection: the infected seedlings and scions are cultured at 37℃ for 28-40 days to obtain virus-free seedlings; Or put the bud strips in 70℃ hot air for 10 minutes to obtain virus-free bud strips; Rootstock can also be propagated from seed.Chemical control:In spring, spray 1.5% plant disease cream 1000 times liquid, or 20% morine hydrochloride · copper wettable powder 4000 times liquid, or 0.05-0.1% rare earth nitrate once every 15-20 days, for 2-3 consecutive times. Spray the fruit once more before harvest; It can also be injected 1-2 times with 0.05-0.1% rare earth solution before and after germination, and the amount of plant is 0.5-1 kg.Biological interference: The use of weak strains of apple Mosaic virus can play a role in reducing the harm. ",16,0);
        break;
        case 'g': Show_Str(10,105,BLACK,0xFFF8,"Agricultural control:1. Eradicate cypress apple production areas prohibit the planting of cypress, if there are cypress in scenic areas, it is not appropriate to develop apple orchards, should be at least 5 kilometers away. Areas where cypress has been sportily planted are recommended to be removed to prevent the occurrence of apple rust.2. Spring prevention and treatment of juniper Juniper: to check the gall and glue flower, timely cut off and destroy. When the apple sprouts to the size of the thumb cap of the young fruit, spray 1-2 degrees stone sulfur mixture on the juniper trees, and spray the whole tree 1-2 times.Chemical control:1. Spray protection: Starting from the apple leaf period, spray a fungicide every 10-15 days for 2-3 consecutive times to protect the leaves from rust. Or before a large range of precipitation, spray 1 fungicide to prevent rust bacteria from infecting during rainfall.2. Spray treatment: In April-May, if the rainfall exceeds 5 mm and lasts for more than 12 hours, spray the internal bactericide within 5 days after the rainfall. When a large number of winter spore horns germinated, that is, within 5 days after flowering of cypress trees, an internal fungicide was sprayed. At the beginning of the disease of apple leaves, that is, when the red spots appear, immediately spray the internal fungicide. Commonly used special agents include mancozeb, fluosilazole, phenacetoconazole, methylthiobacillam, triazolone, pentazolol, propionazole, nitrile, etc. The spray concentration is determined according to the actual situation.",16,0);
        break;
        case 'h': Show_Str(10,105,BLACK,0xFFF8,"In the prevention and control of apple black star disease, we should adhere to the principle of prevention and treatment, in order to effectively control the occurrence of the disease and reduce the harm.Agricultural control:1. Reasonable pruning: Ensuring good permeability of the orchard is the key, including winter pruning to control the amount of mu branches, spring wiping out useless buds, summer cutting off multiple branches, autumn thinning useless large branches.2. Maintain a healthy tree: increase fertilizer and water, control the amount of fruit, ensure the healthy growth of the tree, and improve the ability to resist black star disease.3. Garden cleaning work: winter sweep leaves, remove weeds in the garden, spring scrape branch canker spot, spray stone sulfur mixture to reduce the number of field pathogens, reduce the occurrence of black star disease.Chemical control:1. Before the infection of the bacteria in early May, spray high-efficiency protective fungicides such as polyamycin, agricultural resistance 120, Dupont Fuxing, etc.2. After the appearance of disease symptoms, spray with treatment and eradication of fungicides such as Longdengfulan liquid, propionoconazole, bacteriotoxin, pentazolol, Shigol, phenacetoconazole, etc., lasting and stable efficacy.",16,0);
        break;
        case 'i': Show_Str(10,105,BLACK,0xFFF8,"Your crops are healthy!",16,0);
        break;
        case 'j': Show_Str(10,105,BLACK,0xFFF8,"1 Choose 5% Chlorothalonil dust agent or spray 0.3 at the beginning of the disease. Be stone sulfur mixture or 25% powder rust ning 3000 times liquid, 70% methylthiobacillam wettable powder 1500 times liquid l once or twice.2 Clean up the orchard after autumn, sweep the fallen leaves, and burn them in a concentrated way.",16,0);
        break;
        case 'k': Show_Str(10,105,BLACK,0xFFF8,"Agricultural prevention and control: First of all, a seedling site with high terrain, good drainage and sufficient sunshine should be selected, and the soil should be treated before breeding, such as the use of formalin to spray the bed surface, or the seed mixing double wettable powder. Strengthen soil management, avoid low temperature and high humidity conditions, avoid the repeated use of the same plot of seedlings, use fertile, loose, disease-free new loam, apply evenly decomposed fertilizer, sow evenly, water according to soil moisture and weather conditions, keep the bed soil moisture appropriate, and do a good job of heat preservation and ventilation.Chemical control: Spray Bordeaux liquid or other agents, such as methyltobuzin, turbutec, carbendazim, etc., three times a month before the onset of the disease. In rainy years, another spray can be given in August. Other optional agents include chlordebaol gum, methyl thiobacillam, mancozeb, mixed sulfur, prohydantoin, Polyion, viruside alum, etc., according to the duration of the disease every 20 days, three or four times.",16,0);
        break;
        case 'l': Show_Str(10,105,BLACK,0xFFF8,"Agricultural control:1. Eradicate cypress apple production areas prohibit the planting of cypress, if there are cypress in scenic areas, it is not appropriate to develop apple orchards, should be at least 5 kilometers away. Areas where cypress has been sportily planted are recommended to be removed to prevent the occurrence of apple rust.2. Spring prevention and treatment of juniper Juniper: to check the gall and glue flower, timely cut off and destroy. When the apple sprouts to the size of the thumb cap of the young fruit, spray 1-2 degrees stone sulfur mixture on the juniper trees, and spray the whole tree 1-2 times.Chemical control:1. Spray protection: Starting from the apple leaf period, spray a fungicide every 10-15 days for 2-3 consecutive times to protect the leaves from rust. Or before a large range of precipitation, spray 1 fungicide to prevent rust bacteria from infecting during rainfall.2. Spray treatment: In April-May, if the rainfall exceeds 5 mm and lasts for more than 12 hours, spray the internal bactericide within 5 days after the rainfall. When a large number of winter spore horns germinated, that is, within 5 days after flowering of cypress trees, an internal fungicide was sprayed. At the beginning of the disease of apple leaves, that is, when the red spots appear, immediately spray the internal fungicide. Commonly used special agents include mancozeb, fluosilazole, phenacetoconazole, methylthiobacillam, triazolone, pentazolol, propionazole, nitrile, etc. The spray concentration is determined according to the actual situation.",16,0);
        break;
        case 'm': Show_Str(10,105,BLACK,0xFFF8,"Your crops are healthy!",16,0);
        break;
        case 'n': Show_Str(10,105,BLACK,0xFFF8,"Measurement and reporting system: in areas where large spot disease occurs, special personnel are responsible for regular observation of the disease, and once the disease is found, especially the vertical expansion of the disease leaf and high humidity, rain is predicted, timely warning should be issued to guide farmers to take prevention and control measures.Agricultural prevention and control: climatic conditions are the key factors affecting the occurrence of big spot disease. Medium temperature and high humidity are the main climatic conditions of corn big spot disease, so it is necessary to choose the appropriate place and time to plant corn to prevent the disease.Actively promote resistant varieties: Resistant varieties are essential for controlling large spot disease. In common areas, the selection of disease-resistant varieties and reasonable collocation and rotation can effectively reduce the occurrence of diseases.Improving the cultivation environment: implementing measures such as crop rotation, planting wide and narrow rows, interplanting tall and dwarf crops to improve the field environmental conditions and improve the growth quality of maize. After harvest, applying full bottom fertilizer, optimizing fertilization scheme, spreading mulch and timely sowing can reduce the harm of maize big spot.Chemical control: According to the severity of the disease, remove the affected leaves in time, and spray fungicide. Spray once every 10 days for 2-3 consecutive times. Different agents can be selected, such as Haosujing, Sukejing, carbendazim, etc., need to choose the appropriate agent and use method according to the actual situation.",16,0);
        break;
        case 'o': Show_Str(10,105,BLACK,0xFFF8,"Apple black rot is not serious and does not need to be treated alone.Agricultural control: Plant resistant varieties. When cleaning the orchard, pay attention to the pruning branches, diseased fallen leaves, and diseased dead fruits out of the orchard and burn them. Strengthen cultivation management, enhance tree potential, improve disease resistance.Chemical control: Combined with the prevention and treatment of other rot-fruit diseases, from the beginning of the spraying of 40% Dafu Dan wettable powder 450-500 times liquid, or 40% gram Dan wettable powder 450-500 times liquid, or 40% germicidal Dan wettable powder 450-500 times liquid, or 50% mixed sulfur killing suspension 500 times liquid, or 36% methyl thiobacilam suspension 500-600 times liquid, Or 50% Benomycin wettable powder 1500 times liquid. Spray once every 10-14 days, 2-3 times in a row. ",16,0);
        break;
        case 'p': Show_Str(10,105,BLACK,0xFFF8,"1. The disinfection of seedlings is an important part, and the selection of disease-free seedlings or disinfection treatment is necessary. Ammonium sulfate, copper sulfate, ferrous sulfate and other disinfectants can be used to soak the seedlings after planting or breeding.2. Thorough cleaning of the garden can effectively reduce the infection of the disease. When pruning in winter, it is necessary to cut off the diseased branches and the remaining diseased fruit, and completely remove the diseased remains in the orchard, and then burn them in a concentrated way. Spraying eradicants can further remove the remaining germs.3. Selection of disease-resistant varieties is one of the keys to control, and disease-resistant varieties such as Jufeng are a good choice, and suitable varieties are selected according to local conditions.4. Strengthening field management, including maintaining a good ventilated and transparent environment, reasonable fertilization management and timely weeding and pruning, is helpful to improve the disease resistance of plants.5. Fruit bagging is an effective control method, which can prevent the direct contact between the ear and the pesticide and improve the quality of the fruit.6. Regular spraying is also an essential measure, spraying time should be selected before and after the flower and other critical periods, the selection of appropriate agents to spray, help control the development of grape blackpox disease.",16,0);
        break;
        case 'q': Show_Str(10,105,BLACK,0xFFF8,"Your crops are healthy!",16,0);
        break;
        case 'r': Show_Str(10,105,BLACK,0xFFF8,"1. Strengthen field management: control the application of nitrogen fertilizer, do a good job in winter, and thoroughly remove the diseased leaves of the vineyard; At the end of the pompom stage, an eradication agent is applied on the ground to kill overwintering pathogens.2. Pharmaceutical control: The use of pesticides to prevent and control blackpox and anthrax can be treated.",16,0);
        break;
        case 's': Show_Str(10,105,BLACK,0xFFF8,"1. Select Lincheng Peach, Okubo, Yamato White peach, Nakayama Golden Peach, Kurata Haisheng, Cantao No. 2 disease-resistant peach varieties.2, after the start of spring, pay attention to drainage, to achieve the rain stop water dry, reduce air humidity. Increase the application of organic fertilizer and phosphate and potassium fertilizer, avoid partial nitrogen fertilizer.3. Appropriately increase the amount of thinning in the bore, improve the ventilation and light transmission conditions, promote the growth of the tree and improve the resistance to disease. ④ In the peach dormant period from October to November, it is also the pathogen that begins to overwinter on the injured branches, combined with winter garden pruning, completely cut off dead branches and diseased tips, timely clean up the fallen leaves, fallen fruits, etc., centralized burning, and eliminate the source of overwintering bacteria. Stone fruit trees such as apricots and plums should be avoided near peach orchard.4, green environmental protection and pollution-free Chinese medicine control: early spring sprout sprout period spray 300 times + silicone; Spray 200-300 times liquid from the peach tree after the fall of flowers, spray once every 10-15 days, even spray 3-4 times.5, spray before germination 5 degree stone sulfur mixture, or 1∶1∶100 Bordeaux liquid to eradicate the source of overwintering bacteria. After germination, spray 72% agricultural streptomycin sulfate wettable powder 3000 times liquid. Young fruit spray zinc 600 times liquid, or agricultural streptomycin sulfate 4000 times liquid or zinc sulfate lime liquid (zinc sulfate 0.5 kg, slaked lime 2 kg, water 120 kg). Spray once in late June to early July, once every half month to 20 days, and twice or three times.",16,0);
        break;
        case 't': Show_Str(10,105,BLACK,0xFFF8,"Your crops are healthy!",16,0);
        break;
        case 'u': Show_Str(10,105,BLACK,0xFFF8,"Agricultural measures: ① Before planting pepper, the planning and basic construction of pepper garden should be done well; (2) Select disease-free seedlings, strictly prohibit the introduction of seedlings from disease areas; ③ Strengthen the management of pepper garden. Chemical control: spray 1% Bordeaux solution, remove the diseased leaves and their surrounding leaves before spraying.",16,0);
        break;
        case 'v': Show_Str(10,105,BLACK,0xFFF8,"Your crops are healthy!",16,0);
        break;
        case 'w': Show_Str(10,105,BLACK,0xFFF8,"Agricultural control :1. Select early-maturing and disease-resistant varieties, especially high-yield and excellent varieties targeted at local major diseases, and harvest mature crops in time.2. Strengthen tillage management, give priority to plots with fertile soil and good drainage for planting, increase the application of organic fertilizer, and improve the disease resistance of plants. Strengthen land management, increase the amount of potassium fertilizer application, timely irrigation, and promote the growth of plants. Remove disease residues from the field to reduce the source of the first infection of the disease.3. Store crops properly. Harvest mature produce and minimize damage during harvesting and transportation, ensuring that diseased produce does not enter the storage room. The storage temperature is controlled at about 4 ° C, and ventilation is ensured to keep the air fresh.4. Dig deep to reduce the source of primary infection. Remove diseased produce before planting to avoid mixing it into sowing materials.5. Avoid crop rotation or planting in close proximity to nightshade crops.Chemical control:Before the disease occurs, 25% amiceda or 70% mancozeb and other agents can be used to alternately spray prevention, spraying once every 7 to 10 days, continuous prevention of 2 to 3 times. After the onset of the disease, 50% supraline, 70% mancozeb, 50% prohain, 70% methyl tolbuzine and other agents can be used for spraying, spraying once every 7 days, continuous spraying 2 to 3 times according to the situation. In order to avoid drug resistance and improve the effectiveness of control, it is recommended to rotate the above agents.",16,0);
        break;
        case 'x': Show_Str(10,105,BLACK,0xFFF8,"Your crops are healthy!",16,0);
        break;
        case 'y': Show_Str(10,105,BLACK,0xFFF8,"Agricultural control:1. Variety selectionThe threat of late blight to potato varies with different varieties, and the promotion of disease-resistant varieties is an effective way to reduce the threat of epidemic. Although resistant varieties are the basis of late blight control, the resistance of varieties is constantly changing due to genetic variation and external factors. Long-term planting of the same resistant variety may lead to weakening or loss of resistance, resulting in the spread of late blight and loss of yield.2. Crop rotation managementSelect crop rotation plots with a planting period of more than 3 years and avoid planting in areas prone to late blight. No continuous cropping or interplanting with nightshade or cruciferous plants, especially not with tomatoes.3. Field managementPlanting in fields with loose soil and good drainage can promote healthy growth and enhance plant resistance to disease. Strengthen field observation before and after flowering, find the central diseased plant in time, immediately pull it out, and remove the diseased leaves on nearby plants, bury them deep and cover them with lime.Chemical control:According to meteorological conditions and disease prediction, chemical control should be carried out in disease-prone areas, and the scope of control should be gradually expanded. According to the specific situation and the type of agent, determine the spraying frequency. In the absence of highly resistant varieties, chemical control is the main means to control late blight. At the beginning of the disease, the sick plant is removed and the first spray is given, followed by two successive sprays every 7-10 days. Metalaxyl manganese zinc, viruside alum, mancozeb, Clo or Anke manganese zinc can be selected, in order to prevent the development of resistance to bacteria, it is best to alternate the use of different types of agents.",16,0);
        break;
        case 'z': Show_Str(10,105,BLACK,0xFFF8,"1. Seed selection and seed disinfection: Use disease-free seeds. Soak the seeds in warm water at 45℃ for 10 minutes to disinfect the seeds.2. Strengthen cultivation management: Remove the disease residue after harvest, carry out deep cultivation in the heavily affected plots, and carry out crop rotation for more than 2 years if conditions permit.(3) Drug control: Spraying 77% can kill 500 times of wettable particle powder at the beginning of the disease, 64% of virulent alum wettable powder 500 times of liquid, 60% of aluminum wettable powder 500 times of liquid, every 7 to 10 days, prevention and control 1 to 2 times. ",16,0);
        break;
        case 'A': Show_Str(10,105,BLACK,0xFFF8,"1. Clean the garden2. Remove the diseased remains in the field after harvest, and burn them centrally.3. Chemical protection, at the early stage of the disease, spray 50% of methyl tobuzin or 70% of methyl tobuzin 1000 times of the liquid, 50% of carbendazim 800-1000 times of the liquid, or 65% of zinc 500 times of the liquid, every 7-10 days to spray once, a total of three times, have good control effect.4. Choose disease-resistant varieties of broad beans to plant disease-resistant, early-maturing and high-yield varieties to avoid the peak of disease; Cowpea Choose short cowpea with strong resistance to disease.5. Improve cultivation techniques6. To implement crop rotation, low-lying plots should be raised, deep trenches should be opened, suitable for early planting, or according to the specific local conditions, change the planting period.",16,0);
        break;
        case 'B': Show_Str(10,105,BLACK,0xFFF8,"Your crops are healthy!",16,0);
        break;
        case 'C': Show_Str(10,105,BLACK,0xFFF8,"Your crops are healthy!",16,0);
        break;
        case 'D': Show_Str(10,105,BLACK,0xFFF8,"Agricultural control: Take comprehensive measures to strengthen the management of fruit trees, including clearing the garden, eliminating the source of overwintering disease, selecting appropriate agents, mastering the timing and liquid amount of spraying, etc., to improve the control effect. Strengthening cultivation management, increasing the application of organic fertilizer and biological fertilizer, comprehensive fertilization to enhance tree potential and disease resistance, reduce the occurrence of brown spot. Timely pruning, improve ventilation and light, reduce humidity, do a good job of drainage, reduce brown spot occurrence environment. Timely thinning of flowers and fruits, removal of fallen leaves, fruit bagging and other measures also help reduce disease.Chemical control: pentazolol, propiconazole, Ningnanmycin, polyantimycin, Nongkang-120 and other agents can be selected, and the tree body should be fully covered when spraying, paying attention to the back of the leaves and the lower crown of the tree. Prophylactic agents such as 70% Propyl zinc, 80% Dasheng M-45 and 68.75% Ebao 1200 can be used before the onset of the disease. The initial stage of the disease and the accumulation stage can be used alternately with internal therapeutic agents, such as 43% pentazolol suspension, 40% fluosilazole, 40% nitrilazole and so on. During the blooming period, the leaves can be protected with Bordeaux liquid or donin.",16,0);
        break;
        case 'E': Show_Str(10,105,BLACK,0xFFF8,"Agricultural control:1. Select early-maturing and disease-resistant varieties, especially high-yield and excellent varieties targeted at local major diseases, and harvest mature crops in time.2. Strengthen tillage management, give priority to plots with fertile soil and good drainage for planting, increase the application of organic fertilizer, and improve the disease resistance of plants. Strengthen land management, increase the amount of potassium fertilizer application, timely irrigation, and promote the growth of plants. Remove disease residues from the field to reduce the source of the first infection of the disease.3. Store crops properly. Harvest mature produce and minimize damage during harvesting and transportation, ensuring that diseased produce does not enter the storage room. The storage temperature is controlled at about 4 ° C, and ventilation is ensured to keep the air fresh.4. Dig deep to reduce the source of primary infection. Remove diseased produce before planting to avoid mixing it into sowing materials.5. Avoid crop rotation or planting in close proximity to nightshade crops.Chemical control:Before the disease occurs, 25% amiceda or 70% mancozeb and other agents can be used to alternately spray prevention, spraying once every 7 to 10 days, continuous prevention of 2 to 3 times. After the onset of the disease, 50% supraline, 70% mancozeb, 50% prohain, 70% methyl tolbuzine and other agents can be used for spraying, spraying once every 7 days, continuous spraying 2 to 3 times according to the situation. In order to avoid drug resistance and improve the effectiveness of control, it is recommended to rotate the above agents.",16,0);
        break;
        case 'F': Show_Str(10,105,BLACK,0xFFF8,"Your crops are healthy!",16,0);
        break;
        case 'G': Show_Str(10,105,BLACK,0xFFF8,"1 Choose disease-resistant varietiesThe varieties with high resistance to leaf mildew are Jiafang 15, Jiafang 16, Jiafang 17, Zhongza 7, Shenfang 3, Jiafang 15, etc., which can be planted according to local conditions.2 Seed treatment before sowingSoak the seeds in warm water at 55℃ for 30min to remove the pathogens inside and outside the seeds. After treatment, it is best to rinse with clean water several times to remove the pathogens attached to the initial water and seed surface, and then dry sowing.3. Arrange crop rotation in the shed reasonablyTomato cultivation in greenhouses, it is best to arrange a three-year rotation with melons or other nightshade vegetables, which can effectively eliminate the remaining bacteria sources in the soil.4 Greenhouse disinfectionIn the shed where crop rotation is difficult, before planting, 0.25kg sulfur powder and 0.50kg sawdust are mixed per 37m3 space, lit in several places, closed and fumigated for 24h sterilization, and then planted a day later, or 45% Chlorothonil smoke agent is used to fumigate 0.25kg per 110m2 for a day and night for indoor and soil surface disinfection.5. Strengthen cultivation managementStrengthen the temperature and humidity management in the shed, timely ventilation, proper control of watering, timely ventilation and dehumidification after watering, control irrigation after continuous rain and disease. Reasonable dense planting, timely rectification of branches, to facilitate ventilation and light. Implement formula fertilization, avoid excessive nitrogen fertilizer, and appropriately increase phosphorus and potassium fertilizer. Remove the diseased leaves in time when they first appear, and bury or burn them intensively. 6 High temperature stuffy shed choose sunny noon time, closed greenhouse temperature rise to 30 ~ 33℃, and maintain about 2 hours, and then timely ventilation and cooling, the original disease control effect is better.",16,0);
        break;
        case 'H': Show_Str(10,105,BLACK,0xFFF8,"Agricultural control1. Remove the diseased remains in time after harvesting, burn them away from the vegetable fields and water sources, or dig pits for deep burial. Before raising seedlings, the seedbed should be thoroughly cleared of dead branches, leaves and weeds. Dig the soil deeply before planting to promote the decay of the diseased remains; The tools and racks used in the diseased fields should be disinfected.2. Choose a field with convenient drainage and irrigation, open a good drainage ditch, reduce the underground water level, and reach the end of the rain without standing water; It is an important measure for disease prevention to clean the trench system in time after heavy rain to prevent moisture retention and reduce field humidity.3. Use compost or organic fertilizer with fermentation bacteria retting, no bacterial fertilizer, organic fertilizer should not contain plant disease residues; It is beneficial to reduce the disease by applying the formula fertilization technique, increasing the application of phosphate and potassium fertilizer, strengthening the field management, cultivating strong seedlings and enhancing the disease resistance of the plants.4. Water should be scientifically watered during high temperature and drought to improve field humidity, reduce the harm of aphids and grey planthopper and spread other viruses, and take measures such as spreading grass moisturizing between tomato rows, which is also conducive to reducing disease. Continuous irrigation and flood irrigation are strictly prohibited.5. According to local conditions, reasonable selection of disease-resistant varieties. Do not grow next to tobacco.6. When pruning, pruning, and tying vines, healthy plants are first followed by diseased ones. Hands that have come into contact with diseased plants should be disinfected with soapy water to prevent re-transmission through farming operations.",16,0);
        break;
        case 'I': Show_Str(10,105,BLACK,0xFFF8,"Agricultural control: one is to eradicate weeds on the edge of the field, remove residues and leaves, which can eliminate some insect sources and early spring damage objects; Second, when the weather is dry, pay attention to irrigation and increase the humidity of vegetable fields, which is not conducive to its development and reproduction.Chemical control: With 1.8% Nongke mite cream 2000 times liquid, 40% hydrothiophosphate cream 2500 liquid, 20% mite cream 2000 times liquid, 70% grams mite cream 2000 times liquid, 20% diformamidine cream 1000 ~ 1500 times liquid spray control, every 7 ~ 10 days spray 1 time.",16,0);
        break;

    }
}

void Page_Two(void)
{

    while (page == 2) {

    char tem[5],hum[5],pro[5],des[30];

    const char *description = get_uart_flag_string(uart_flag);  // 调用函数获取字符串描述
    sprintf(des, "%c\n", description);  // 打印获取到的描述字符串
    Show_Str(118,65,BLACK,YELLOW,get_uart_flag_string(uart_flag),16,0);
    get_suggestion_string(uart_flag);

    sprintf(tem, "%.2f", temperature);  // %.2f 表示保留两位小数
    Show_Str(100,10,BLACK,YELLOW,tem,16,0);

    sprintf(hum, "%.2f", humidity);
    Show_Str(270,10,BLACK,YELLOW,hum,16,0);

    sprintf(pro, "%.2f%%", probability*100);
    Show_Str(80,88,BLACK,YELLOW,pro,16,0);

    if (temperature>5&temperature<40) {

         Show_Str(52,45,BLACK,0xFFF8,"The soil temperature is normal",16,0);

     }
 //    else if (condition) {
 //
 //    }
 //    else if (condition) {
 //
 //    }
 //    else if (condition) {
 //
 //    }
     if (humidity>80) {

         Show_Str(52,32,BLACK,0xFFF8,"Humidity too high, drainage",16,0);

     }
     else if (humidity>50) {

         Show_Str(52,32,BLACK,"Soil moisture is normal",pro,16,0);

     }

     else if (humidity>30) {

         Show_Str(52,32,BLACK,0xFFF8,"drought,pay attention to irrigation",16,0);

     }
     else {
         Show_Str(52,32,BLACK,0xFFF8,"drought,irrigate immediately",16,0);
     }


     }
 }

void Test_Color(void)
{
    spi_lcd_init(20);
    LCD_Clear(WHITE);
    Gui_Drawbmp32(0,0,320,240,page_1);
    Show_Str(x, y, RED, WHITE, "↑", 16, 0);

    while (page == 1) {
//        button_read();
        if (sele == 1) {

            Show_Str(x, y, WHITE, WHITE, "↑", 16, 0);
            Show_Str(x = x-75, y, RED, WHITE, "↑", 16, 0);
            if (x-75 == 0) {
                x = 40;
            }
            }

        else if (sele == 2) {
            if (y == 215) {
            Show_Str(265, 100, WHITE, WHITE, "↑", 16, 0);
                       }

            Show_Str(x, y, WHITE, WHITE, "↑", 16, 0);

            Show_Str(x = x+75, y, RED, WHITE, "↑", 16, 0);
            if (x == 265) {
                x = -35;
                y = 215;
            }

        }
        else if (sele == 3) {
            if (page == 2) {
                LCD_Clear(WHITE);
                //第一页

            }
        }
        else if (sele == 4) {
            if (y== 100) {
                switch(x){
                case 40:
                    uart_data_send("a");
                   break;

                case 115:
                    uart_data_send("b");
                    break;

                case 190:
                    uart_data_send("d");
                    break;

                case 265:
                    uart_data_send("i");
                    break;
                }
            }

            else if (y== 215) {
                switch(x){
                case 40:
                    uart_data_send("h");
                          break;

                case 115:
                    uart_data_send("f");
                        break;

                case 190:
                    uart_data_send("k");
                    rt_kprintf("11");
                        break;

                case 265:
                    uart_data_send("l");
                        break;
            }
        }
            LCD_Clear(WHITE);
            Gui_Drawbmp32(0,0,320,240,page_2);
            page = 2;
        }
        Page_Two();
        sele = 0;
        rt_thread_delay(200);  // 每隔1秒执行一次

    }
    }


